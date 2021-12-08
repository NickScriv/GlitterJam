// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ChasePlayer.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tasks/AITask_MoveTo.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavMesh/RecastNavMesh.h"
//#include "DrawDebugHelpers.h"

EBTNodeResult::Type UBTTaskNode_ChasePlayer::ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	
	if (!OwnerComp.GetAIOwner())
		return EBTNodeResult::Failed;

	//DrawDebugSphere(GetWorld(), OwnerComp.GetBlackboardComponent()->GetValueAsVector(bbKey_Loc.SelectedKeyName), 50, 26, FColor(181, 181, 181), false, 0.2, 0, 2);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(OwnerComp.GetAIOwner(), OwnerComp.GetBlackboardComponent()->GetValueAsVector(bbKey_Loc.SelectedKeyName));

	return EBTNodeResult::Succeeded;
}