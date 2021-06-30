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


void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (!ensure(AIBlackboard))
	{
		return;
	}

	UseBlackboard(AIBlackboard, blackboardComp);

	if (!ensure(AIBehavior))
	{
		return;
	}

	//AIPerception = FindComponentByClass<UAIPerceptionComponent>();
	
	TArray<UAIPerceptionComponent*> perceptionComponents;
	GetComponents<UAIPerceptionComponent>(perceptionComponents);

	for(UAIPerceptionComponent* perc : perceptionComponents)
	{
		if(perc->GetName() == "AIPerceptionMain")
		{
			UE_LOG(LogTemp, Warning, TEXT("AIPerceptionMain found!"));
			AIPerception = perc;
		}
		else if(perc->GetName() == "AIPerception2")
		{
			UE_LOG(LogTemp, Warning, TEXT("AIPerception2 found!"));
			AIPerceptionStart = perc;
		}
	}

	if (!ensure(AIPerception && AIPerceptionStart))
	{
		return;
	}

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::perceptionUpdated);
	AIPerceptionStart->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::perceptionUpdated);
	

	//RunBehaviorTree(AIBehavior);

}

void AMonsterAIController::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);

	AMonster* monster = Cast<AMonster>(GetPawn());

	if(!monster)
		return;

	FTransform trans = monster->GetMesh()->GetSocketTransform("Eyes");

	OutLocation = trans.GetLocation();
	OutRotation = trans.GetRotation().Rotator();
}

void AMonsterAIController::perceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	AMainCharacter* player = Cast<AMainCharacter>(Actor);
	
	//UE_LOG(LogTemp, Warning, TEXT("%s"), Stimulus.WasSuccessfullySensed() ? TEXT("True") : TEXT("False"));

	FString senseName = UKismetSystemLibrary::GetDisplayName(UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus));

	if (senseName == TEXT("AISense_Sight"))
	{
		if(!firstSeen && !Stimulus.WasSuccessfullySensed())
		{
			firstSeen = true;
			AIPerceptionStart->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
			AIPerception->SetSenseEnabled(UAISense_Sight::StaticClass(), true);
			AIPerceptionStart->SetSenseEnabled(UAISense_Hearing::StaticClass(), false);
			AIPerception->SetSenseEnabled(UAISense_Hearing::StaticClass(), true);
			UE_LOG(LogTemp, Warning, TEXT("Change AIPerception"));
			
		}
		blackboardComp->SetValueAsBool(FName("CanSeePlayer"), Stimulus.WasSuccessfullySensed());
		
	}
	else if (senseName == TEXT("AISense_Hearing"))
	{
		FHitResult hit;
		FCollisionQueryParams qParams;
		
		if (GetWorld()->LineTraceSingleByChannel(hit, GetPawn()->GetActorLocation(), Stimulus.StimulusLocation, ECC_Visibility, qParams))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit raycast to player: %s"), *hit.Actor->GetName());
			if (Cast<AMainCharacter>(hit.Actor))
			{
				blackboardComp->SetValueAsBool(FName("IsInvestigating"), Stimulus.WasSuccessfullySensed());
				blackboardComp->SetValueAsVector(FName("TargetLoc"), Stimulus.StimulusLocation);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("None"));
	}

}

void AMonsterAIController::StartMonsterBehavior()
{
	
	RunBehaviorTree(AIBehavior);
	
	AMonster* monster = Cast<AMonster>(GetPawn());

	if (!monster)
		return;
	//monster->TracePath();
}
