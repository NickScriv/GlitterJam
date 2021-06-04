// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerInRange.h"
#include "Monster.h"
#include "MainCharacter.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_PlayerInRange::UBTService_PlayerInRange()
{
	bNotifyBecomeRelevant = 1;
}

void UBTService_PlayerInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp,  NodeMemory);

	if (!OwnerComp.GetAIOwner())
		return;

	AMonster* monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!monster)
		return;

	if (AMainCharacter* player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(bbKey_InRange.SelectedKeyName, FVector::DistSquared(player->GetActorLocation(), monster->GetActorLocation()) <= killDistance * killDistance);
		
	
	}

}


