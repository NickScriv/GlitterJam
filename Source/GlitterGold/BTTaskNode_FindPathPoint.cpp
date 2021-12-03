// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_FindPathPoint.h"
#include "Monster.h"
#include "AIController.h"
#include "PathPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"


EBTNodeResult::Type UBTTaskNode_FindPathPoint::ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if(!OwnerComp.GetAIOwner())
		return EBTNodeResult::Failed;

	AMonster* monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if(!monster)
		return EBTNodeResult::Failed;

	int32 index = monster->currentPathIndex;
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(bbKey_TargetLoc.SelectedKeyName, monster->currentPath[index]->GetActorLocation());
	//OwnerComp.GetBlackboardComponent()->SetValueAsObject("PathPointActor", monster->currentPath[index]);

	float wait = FMath::RandRange(monster->currentPath[index]->waitTime - monster->currentPath[index]->waitTimeDeviation, monster->currentPath[index]->waitTime + monster->currentPath[index]->waitTimeDeviation);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(bbKey_PathPointWaitTime.SelectedKeyName, wait);

	return EBTNodeResult::Succeeded;


}
