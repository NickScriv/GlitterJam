// Fill out your copyright notice in the Description page of Project Settings.


#include "GlitterGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "HUDUserWidget.h"
#include "KillScreenUserWidget.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"



void AGlitterGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	HUDWidget = CreateWidget<UHUDUserWidget>(GetGameInstance(), HUDWidgetClass);
	HUDWidget->AddToViewport();
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
