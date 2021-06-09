// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_MonsterSounds.h"
#include "Monster.h"
#include "AIController.h"

UBTService_MonsterSounds::UBTService_MonsterSounds()
{
	bNotifyBecomeRelevant = 1;
}

void UBTService_MonsterSounds::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
		return;

	AMonster* monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!monster)
		return;

	monster->PlayMosnterSoundEvent(monsterSound);

	
}



