// Fill out your copyright notice in the Description page of Project Settings.


#include "GlitterGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "HUDUserWidget.h"
#include "KillScreenUserWidget.h"
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
