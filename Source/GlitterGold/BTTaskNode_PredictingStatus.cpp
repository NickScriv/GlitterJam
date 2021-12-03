// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_PredictingStatus.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "MonsterAIController.h"

EBTNodeResult::Type UBTTaskNode_PredictingStatus::ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(bbKey_Status.SelectedKeyName, status);

	if (!OwnerComp.GetAIOwner())
		return EBTNodeResult::Failed;

	AMonsterAIController* monstController = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if(!monstController)
		return EBTNodeResult::Failed;

	monstController->canHear = true;

	return EBTNodeResult::Succeeded;

}