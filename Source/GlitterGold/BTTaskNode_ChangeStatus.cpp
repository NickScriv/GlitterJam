// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ChangeStatus.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTaskNode_ChangeStatus::ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(bbKey_Status.SelectedKeyName, (uint8)targetStatus);

	return EBTNodeResult::Succeeded;
}

