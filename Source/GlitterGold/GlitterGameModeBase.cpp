// Fill out your copyright notice in the Description page of Project Settings.


#include "GlitterGameModeBase.h"

#include "AkAudioDevice.h"
#include "Blueprint/UserWidget.h"
#include "MainCharacter.h"
#include "HUDUserWidget.h"
#include "KillScreenUserWidget.h"
#include "PauseMenuUserWidget.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "ManageEvents.h"

void AGlitterGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	HUDWidget = CreateWidget<UHUDUserWidget>(GetGameInstance(), HUDWidgetClass);
	HUDWidget->AddToViewport();
	TArray<AActor*> eventManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AManageEvents::StaticClass(), eventManagers);
	pauseScreenWidget = CreateWidget<UPauseMenuUserWidget>(GetGameInstance(), pauseWidgetClass);

	if(eventManagers.Num() > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("There should be only 1 event manager!!!!!"));
	}
}

void AGlitterGameModeBase::AddNotification(FText content)
{
	
	HUDWidget->PopNotification(content);
}

void AGlitterGameModeBase::EndGame()
{
	HUDWidget->RemoveFromParent();

	// TODO: Take this out for polish
	KillScreenWidget = CreateWidget<UKillScreenUserWidget>(GetGameInstance(), killScreenWidgetClass);
	KillScreenWidget->AddToViewport();
	
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void AGlitterGameModeBase::TogglePause(AMainCharacter* character)
{
	if(isReadingNote)
		return;
	
	isPaused = !isPaused;
	UGameplayStatics::SetGamePaused(GetWorld(), isPaused);
	UE_LOG(LogTemp, Warning, TEXT("outside pause"));
	if(isPaused)
	{
		UE_LOG(LogTemp, Warning, TEXT("Add to view pause"));
		// TODO: Add pause event to pause all other sounds
		FAkAudioDevice::Get()->PostEvent("Pause_Menu_On", character);
		pauseScreenWidget->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Add to view unpause"));
		// TODO: Add unpause event to unpause all other sounds
		FAkAudioDevice::Get()->PostEvent("Pause_Menu_Off", character);
		pauseScreenWidget->RemoveFromParent();
	}
}

