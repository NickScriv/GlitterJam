// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ReachedPathPoint.h"
#include "Monster.h"
#include "AIController.h"

EBTNodeResult::Type UBTTaskNode_ReachedPathPoint::ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
		return EBTNodeResult::Failed;

	AMonster* monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!monster)
		return EBTNodeResult::Failed;

	if (!monster->currentPath.IsValidIndex(monster->currentPathIndex + monster->pathDirection))
	{
		switch (monster->pathEnding)
		{
			case EPathEnding::Nothing:
				return EBTNodeResult::Succeeded;
				break;
			case EPathEnding::Loop:
				if (monster->pathDirection < 0)
				{
					monster->currentPathIndex = monster->currentPath.Num() - 1;
				}
				else
				{
					monster->currentPathIndex = 0;
				}
				break;
			case EPathEnding::PatrolBack:
				monster->pathDirection = -monster->pathDirection;
				break;
		}
	}
	else
	{
		monster->currentPathIndex += monster->pathDirection;
	}

	return EBTNodeResult::Succeeded;
}

