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
		if(perc->GetName() == "AIPerceptionMain")
		{
			//UE_LOG(LogTemp, Warning, TEXT("AIPerceptionMain found!"));
			AIPerception = perc;
		}
		else if(perc->GetName() == "AIPerception2")
		{
			//UE_LOG(LogTemp, Warning, TEXT("AIPerception2 found!"));
			AIPerceptionStart = perc;
		}
	}

	if (!AIPerception || !AIPerceptionStart)
	{
		return;
	}

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::perceptionUpdated);
	AIPerceptionStart->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::perceptionUpdated);
	blackboardComp->SetValueAsInt(FName("ChangePath"), 0);
	blackboardComp->SetValueAsObject(FName("TargetActor"), UGameplayStatics::GetPlayerCharacter(this, 0));

	//RunBehaviorTree(AIBehavior);

}

void AMonsterAIController::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);

	AMonster* monster = Cast<AMonster>(GetPawn());

	if (!monster)
		return;


	FTransform trans = monster->GetMesh()->GetSocketTransform("Eyes");

	OutLocation = trans.GetLocation();
	OutRotation = trans.GetRotation().Rotator();
}

void AMonsterAIController::perceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	AMainCharacter* player = Cast<AMainCharacter>(Actor);

	if (!player)
		return;

	if (!GetPawn())
		return;
	
	//UE_LOG(LogTemp, Warning, TEXT("%s"), Stimulus.WasSuccessfullySensed() ? TEXT("True") : TEXT("False"));

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

		/*FVector eyesLoc = monster->GetMesh()->GetSocketLocation(FName("Eyes"));

		TArray<AActor*> actorsToIgnore;

		actorsToIgnore.Add(monster);
		FHitResult hit;

		bool sensed = false;

		if (Stimulus.WasSuccessfullySensed() && UKismetSystemLibrary::SphereTraceSingle(this, eyesLoc, player->GetActorLocation(), 15.f, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore, EDrawDebugTrace::ForDuration, hit, true, FLinearColor::Red, FLinearColor::Green, 5.0f))
		{
			if (Cast<AMainCharacter>(hit.Actor))
			{
				sensed = true;
			}

		}
		//UE_LOG(LogTemp, Warning, TEXT("%s"), sensed ? TEXT("True") : TEXT("False"));*/

		blackboardComp->SetValueAsBool(FName("CanSeePlayer"), Stimulus.WasSuccessfullySensed());
		
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
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("None"));
	}

}

void AMonsterAIController::SetToDefaultPerception()
{
	if(!firstSeen)
	{
		firstSeen = true;
		AIPerceptionStart->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
		AIPerception->SetSenseEnabled(UAISense_Sight::StaticClass(), true);
		AIPerceptionStart->SetSenseEnabled(UAISense_Hearing::StaticClass(), false);
		AIPerception->SetSenseEnabled(UAISense_Hearing::StaticClass(), true);
		//UE_LOG(LogTemp, Warning, TEXT("Change AIPerception"));
	}
}

void AMonsterAIController::StartMonsterBehavior()
{
	
	RunBehaviorTree(AIBehavior);
	

	AMonster* monster = Cast<AMonster>(GetPawn());

	if (!monster)
		return;

	GetWorldTimerManager().SetTimer(timerHandleFirstSeen, this, &AMonsterAIController::SetToDefaultPerception, ChangePerceptionTime,  false);
	//monster->TracePath();
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
