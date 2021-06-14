// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_MoveToPlayer.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UBTTaskNode_MoveToPlayer::ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!UGameplayStatics::GetPlayerCharacter(this, 0))
		return EBTNodeResult::Failed;



	FNavLocation randomLoc;

	if (!UNavigationSystemV1::GetCurrent(GetWorld())->GetRandomPointInNavigableRadius(UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorLocation(), radius, randomLoc))
	{
		return EBTNodeResult::Failed;
	}


	OwnerComp.GetBlackboardComponent()->SetValueAsVector(bbKey.SelectedKeyName, UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorLocation());

	return EBTNodeResult::Succeeded;
}