// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeMusic.h"
//#include "../Plugins/Wwise/Source/AkAudio/Classes/AkGameplayStatics.h"
#include "Monster.h"
#include "AIController.h"
//#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"

UBTService_ChangeMusic::UBTService_ChangeMusic()
{
	bNotifyBecomeRelevant = 1;
}

void UBTService_ChangeMusic::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
		return;

	AMonster* monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!monster)
		return;
	

	//FAkAudioDevice::Get()->PostEvent(*musicChange, monster);
}


