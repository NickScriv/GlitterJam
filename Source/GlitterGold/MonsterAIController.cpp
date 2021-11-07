// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainCharacter.h"
#include "Monster.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Sight.h"


void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (!AIBlackboard)
	{
		return;
	}

	UseBlackboard(AIBlackboard, blackboardComp);

	if (!AIBehavior)
	{
		return;
	}
	
	TArray<UAIPerceptionComponent*> perceptionComponents;
	GetComponents<UAIPerceptionComponent>(perceptionComponents);

	for(UAIPerceptionComponent* perc : perceptionComponents)
	{

		if(perc->GetName() == "AIPerception2")
		{
			//UE_LOG(LogTemp, Warning, TEXT("AIPerception2 found!"));
			AIPerception = perc;
			break;
		}
	}

	if (AIPerception)
	{
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::perceptionUpdated);
	}

	
	blackboardComp->SetValueAsInt(FName("ChangePath"), 0);
	blackboardComp->SetValueAsObject(FName("TargetActor"), UGameplayStatics::GetPlayerCharacter(this, 0));
	blackboardComp->SetValueAsEnum(FName("MonsterStatus"), (uint8)MonsterStatus::Patrolling); 
}

void AMonsterAIController::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);

	AMonster* monster = Cast<AMonster>(GetPawn());

	if (!monster)
		return;


	FTransform trans = monster->GetMesh()->GetSocketTransform("Eyes");

	OutLocation = trans.GetLocation();
	OutRotation.Yaw = trans.GetRotation().Rotator().Yaw;
}

void AMonsterAIController::perceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!GetPawn())
		return;

	FString senseName = UKismetSystemLibrary::GetDisplayName(UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus));

	if (senseName == TEXT("AISense_Sight"))
	{
		if(!firstSeen && !Stimulus.WasSuccessfullySensed())
		{
			SetToDefaultPerception();
		}

		AMonster* monster = Cast<AMonster>(GetPawn());

		if (!monster)
			return;

		
		blackboardComp->SetValueAsBool(FName("CanSeePlayer"), Stimulus.WasSuccessfullySensed());

		if (Stimulus.WasSuccessfullySensed() && blackboardComp->GetValueAsEnum(FName("MonsterStatus")) == (uint8)MonsterStatus::Patrolling)
		{
			UE_LOG(LogTemp, Warning, TEXT("Monster sensed"));
			blackboardComp->SetValueAsBool(FName("IsScreaming"), true);
			blackboardComp->SetValueAsEnum(FName("MonsterStatus"), (uint8)MonsterStatus::Chasing);
			isScreaming = true;
			monster->GetCharacterMovement()->Deactivate();
		}
	}
	else if (senseName == TEXT("AISense_Hearing"))
	{

		FHitResult hit;
		FCollisionQueryParams qParams;
		qParams.AddIgnoredActor(GetPawn());

		if (!GetWorld())
			return;

		if (GetWorld()->LineTraceSingleByChannel(hit, GetPawn()->GetActorLocation(), Stimulus.StimulusLocation, ECC_Visibility, qParams))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Hit raycast to player: %s"), *hit.Actor->GetName());
			if (Cast<AMainCharacter>(hit.Actor))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Heard Player at: %f, %f, %f"), Stimulus.StimulusLocation.X, Stimulus.StimulusLocation.Y, Stimulus.StimulusLocation.Z);
				blackboardComp->SetValueAsBool(FName("IsInvestigating"), Stimulus.WasSuccessfullySensed());
				blackboardComp->SetValueAsVector(FName("TargetLoc"), Stimulus.StimulusLocation);
			}
		}
	}
	else if (senseName == TEXT("AISense_Damage"))
	{

		//UE_LOG(LogTemp, Warning, TEXT("Damage AI at: %f, %f, %f"), Stimulus.StimulusLocation.X, Stimulus.StimulusLocation.Y, Stimulus.StimulusLocation.Z);
		blackboardComp->SetValueAsBool(FName("IsInvestigating"), Stimulus.WasSuccessfullySensed());
		blackboardComp->SetValueAsVector(FName("TargetLoc"), Stimulus.StimulusLocation);

	}
}

void AMonsterAIController::StopScreaming()
{
	blackboardComp->SetValueAsBool(FName("IsScreaming"), false);
	isScreaming = false;
	AMonster* monster = Cast<AMonster>(GetPawn());
	monster->GetCharacterMovement()->Activate();
}

void AMonsterAIController::SetToDefaultPerception()
{
	if (!firstSeen)
	{
		firstSeen = true;
		FAISenseID id = UAISense::GetSenseID(UAISense_Sight::StaticClass());
		if (!id.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Wrong Sense ID"));
			return;
		}

		auto config = AIPerception->GetSenseConfig(id);
		if (config == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Sight config is nullptr"));
			return;
		}

		auto configSight = Cast<UAISenseConfig_Sight>(config);

		if (configSight)
		{
			configSight->SightRadius = mainSightRadius;
			configSight->LoseSightRadius = mainLoseSightRadius;
			AIPerception->RequestStimuliListenerUpdate();
		}
		
	}
}

void AMonsterAIController::StartMonsterBehavior()
{
	
	RunBehaviorTree(AIBehavior);

	AMonster* monster = Cast<AMonster>(GetPawn());

	if (!monster)
		return;

	GetWorldTimerManager().SetTimer(timerHandleFirstSeen, this, &AMonsterAIController::SetToDefaultPerception, ChangePerceptionTime,  false);
}

void AMonsterAIController::TriggerPatrolAbort()
{
	// Just need to trigger abort without conditional
	if (changePathValue)
	{
		blackboardComp->SetValueAsInt(FName("ChangePath"), blackboardComp->GetValueAsInt(FName("ChanePath")) + 1);
	}
	else
	{
		blackboardComp->SetValueAsInt(FName("ChangePath"), blackboardComp->GetValueAsInt(FName("ChanePath")) - 1);
	}

	changePathValue = !changePathValue;
}
