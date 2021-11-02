// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeBrakingAcc.h"
#include "Monster.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_ChangeBrakingAcc::UBTService_ChangeBrakingAcc()
{
	bNotifyBecomeRelevant = 1;
}

void UBTService_ChangeBrakingAcc::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
		return;

	AMonster* monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!monster)
		return;

	monster->GetCharacterMovement()->BrakingDecelerationWalking = brakeAcc;
}