// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ChasePlayer.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTaskNode_ChasePlayer::ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	
	if (!OwnerComp.GetAIOwner())
		return EBTNodeResult::Failed;

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(OwnerComp.GetAIOwner(), OwnerComp.GetBlackboardComponent()->GetValueAsVector(bbKey_Loc.SelectedKeyName));


	return EBTNodeResult::Succeeded;
}