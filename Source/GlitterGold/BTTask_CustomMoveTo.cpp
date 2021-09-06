// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CustomMoveTo.h"
#include "AIController.h"
#include "Monster.h"

EBTNodeResult::Type UBTTask_CustomMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (NodeResult == EBTNodeResult::Failed)
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveToNode Failed!!!!!"));
	}
	

	return NodeResult;
}
