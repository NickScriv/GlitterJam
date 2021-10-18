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

	pauseScreenWidget = CreateWidget<UPauseMenuUserWidget>(GetGameInstance(), pauseWidgetClass);
}

void AGlitterGameModeBase::AddNotification(FText content)
{
	HUDWidget->PopNotification(content);
}

void AGlitterGameModeBase::ReticleUI(bool show)
{
	HUDWidget->ReticleUI(show);
}

void AGlitterGameModeBase::AmmoUI(bool show, int32 ammoCount)
{
	HUDWidget->AmmoUI(show, ammoCount);
}

void AGlitterGameModeBase::FadeInHUD()
{
	HUDWidget->FadeInHUD();
}

void AGlitterGameModeBase::FadeOutHUD()
{
	HUDWidget->FadeOutHUD();
}

void AGlitterGameModeBase::EndGame()
{
	HUDWidget->RemoveFromParent();

	// TODO: Take this out for polish
	KillScreenWidget = CreateWidget<UKillScreenUserWidget>(GetGameInstance(), killScreenWidgetClass);
	KillScreenWidget->AddToViewport();
	
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void AGlitterGameModeBase::BadEnding()
{
	HUDWidget->ScreenToBlack();
	GetWorldTimerManager().SetTimer(switchToMainMenuHandle, this, &AGlitterGameModeBase::SwitchToMainMenu, 5.f, false);
}

void AGlitterGameModeBase::SwitchToMainMenu()
{
	UGameplayStatics::OpenLevel(this, FName("MainMenu"));
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

		FAkAudioDevice::Get()->PostEvent("Pause_Menu_On", character);
		pauseScreenWidget->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Add to view unpause"));

		FAkAudioDevice::Get()->PostEvent("Pause_Menu_Off", character);
		pauseScreenWidget->RemoveFromParent();
	}
}

