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

	FNavLocation randomLoc;
	FVector target;
	if (UNavigationSystemV1::GetCurrent(GetWorld())->GetRandomPointInNavigableRadius(monster->path[index]->GetActorLocation(), radius, randomLoc))
	{
		target = randomLoc.Location;
	}
	else
	{
		target = monster->path[index]->GetActorLocation();
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(bbKey_TargetLoc.SelectedKeyName, target);
	
	float wait = FMath::RandRange(monster->path[index]->waitTime - monster->path[index]->waitTimeDeviation, monster->path[index]->waitTime + monster->path[index]->waitTimeDeviation);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(bbKey_PathPointWaitTime.SelectedKeyName, wait);

	//OwnerComp.GetBlackboardComponent()->SetValueAsBool(bbKey_Caution.SelectedKeyName, false);

	if(overrideSpeed)
		monster->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	return EBTNodeResult::Succeeded;


}
