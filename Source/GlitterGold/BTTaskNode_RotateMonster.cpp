// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_RotateMonster.h"
#include "Monster.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PathPoint.h"

EBTNodeResult::Type UBTTaskNode_RotateMonster::ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
		return EBTNodeResult::Failed;

	AMonster* monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!monster)
		return EBTNodeResult::Failed;

	APathPoint* pathPoint = monster->currentPath[monster->currentPathIndex];

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnMoveComponentToEnd");

	UKismetSystemLibrary::MoveComponentTo(monster->GetRootComponent(), monster->GetActorLocation(), pathPoint->GetActorRotation(), false, false, 0.2f, false, EMoveComponentAction::Type::Move, LatentInfo);

	
	return EBTNodeResult::InProgress;
}

void UBTTaskNode_RotateMonster::OnMoveComponentToEnd()
{
	//UE_LOG(LogTemp, Warning, TEXT("wow"));

}
