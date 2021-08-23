// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeMusic.h"
//#include "../Plugins/Wwise/Source/AkAudio/Classes/AkGameplayStatics.h"
#include "Monster.h"
#include "AIController.h"
#include "GlitterGameModeBase.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"

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

	AGlitterGameModeBase* gameMode = Cast<AGlitterGameModeBase>(UGameplayStatics::GetGameMode(this));

	if(!gameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("UBTService_ChangeMusic: No game mode found!"));
		return;
	}
	
	if(musicChange == "Play_Ambient_Music")
	{	
		musicChange = gameMode->queuedMusic;
	}

	

	if (AMainCharacter* player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		FAkAudioDevice::Get()->PostEvent(*musicChange, player);
	}
}


