// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


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

	RunBehaviorTree(AIBehavior);
	

	AIPerception = FindComponentByClass<UAIPerceptionComponent>();

	if (!ensure(AIPerception))
	{
		return;
	}

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::perceptionUpdated);


}

void AMonsterAIController::perceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	

	AMainCharacter* player = Cast<AMainCharacter>(Actor);
	
	//UE_LOG(LogTemp, Warning, TEXT("%s"), Stimulus.WasSuccessfullySensed() ? TEXT("True") : TEXT("False"));

	FString senseName = UKismetSystemLibrary::GetDisplayName(UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus));

	if (senseName == TEXT("AISense_Sight"))
	{
		blackboardComp->SetValueAsBool(FName("CanSeePlayer"), Stimulus.WasSuccessfullySensed());
		
	}
	else if (senseName == TEXT("AISense_Hearing"))
	{
		blackboardComp->SetValueAsBool(FName("IsInvestigating"), Stimulus.WasSuccessfullySensed());
		blackboardComp->SetValueAsVector(FName("TargetLoc"), Stimulus.StimulusLocation);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("None"));
	}

}
