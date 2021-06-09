// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "Monster.h"
#include "GameFramework/CharacterMovementComponent.h"


EBTNodeResult::Type UBTTask_RandomLocation::ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if(!OwnerComp.GetAIOwner())
		return EBTNodeResult::Failed;

	AMonster* monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if(!monster)
		return EBTNodeResult::Failed;
	
	FNavLocation randomLoc;

	if (!UNavigationSystemV1::GetCurrent(GetWorld())->GetRandomPointInNavigableRadius(monster->GetActorLocation(), radius, randomLoc))
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(bbKey.SelectedKeyName ,randomLoc.Location);

	return EBTNodeResult::Succeeded;
}
