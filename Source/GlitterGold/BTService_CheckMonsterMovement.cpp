// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckMonsterMovement.h"
#include "Monster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_CheckMonsterMovement::UBTService_CheckMonsterMovement()
{
	bNotifyBecomeRelevant = 1;
}

void UBTService_CheckMonsterMovement::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
		return;

	AMonster* monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!monster)
		return;

	if (!monster->GetCharacterMovement()->IsActive())
	{
		monster->GetCharacterMovement()->Activate();
	}

}