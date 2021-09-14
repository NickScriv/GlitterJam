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
	UE_LOG(LogTemp, Warning, TEXT("Activate Monster 0"));
	if (!OwnerComp.GetAIOwner())
		return;
	UE_LOG(LogTemp, Warning, TEXT("Activate Monster 1"));
	AMonster* monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!monster)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Activate Monster 2"));
	monster->GetCharacterMovement()->Activate();
	

}