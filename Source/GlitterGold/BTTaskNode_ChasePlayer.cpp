// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ChasePlayer.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tasks/AITask_MoveTo.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavMesh/RecastNavMesh.h"

EBTNodeResult::Type UBTTaskNode_ChasePlayer::ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	
	if (!OwnerComp.GetAIOwner())
		return EBTNodeResult::Failed;
	//UE_LOG(LogTemp, Warning, TEXT("Chase player 1"));
	// 
 	/*FNavLocation navLoc;
	FNavigationQueryFilter navFilter;
	const FNavAgentProperties& AgentProps = OwnerComp.GetAIOwner()->GetNavAgentPropertiesRef();
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	
	
	const ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(&navSystem->GetMainNavDataChecked());
	FVector goal = OwnerComp.GetBlackboardComponent()->GetValueAsVector(bbKey_Loc.SelectedKeyName);
	bool navResult = navSystem->ProjectPointToNavigation(goal, navLoc, extents, &AgentProps);

	//bool navResult = NavMesh->ProjectPoint(OwnerComp.GetBlackboardComponent()->GetValueAsVector(bbKey_Loc.SelectedKeyName),
	//navLoc, extents, 0, OwnerComp.GetAIOwner()->GetPawn());
	if (navResult)
	{
		//UE_LOG(LogTemp, Warning, TEXT("UBTTaskNode_ChasePlayer: Could nt project point onto navemesh!!!!"));
		return EBTNodeResult::Failed;
	}
	UE_LOG(LogTemp, Warning, TEXT("Key Loc: %f, %f, %f"), OwnerComp.GetBlackboardComponent()->GetValueAsVector(bbKey_Loc.SelectedKeyName).X, OwnerComp.GetBlackboardComponent()->GetValueAsVector(bbKey_Loc.SelectedKeyName).Y, OwnerComp.GetBlackboardComponent()->GetValueAsVector(bbKey_Loc.SelectedKeyName).Z);
	UE_LOG(LogTemp, Warning, TEXT("NavLoc: %f, %f, %f"), navLoc.Location.X, navLoc.Location.Y, navLoc.Location.Z);
	*/
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(OwnerComp.GetAIOwner(), OwnerComp.GetBlackboardComponent()->GetValueAsVector(bbKey_Loc.SelectedKeyName));

	//UAITask_MoveTo::AIMoveTo(OwnerComp.GetAIOwner(), OwnerComp.GetBlackboardComponent()->GetValueAsVector(bbKey_Loc.SelectedKeyName), nullptr, 2000.0f);
	//UE_LOG(LogTemp, Warning, TEXT("Chase player 2"));

	return EBTNodeResult::Succeeded;
}