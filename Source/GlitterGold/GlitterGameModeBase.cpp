// Fill out your copyright notice in the Description page of Project Settings.


#include "GlitterGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "HUDUserWidget.h"



void AGlitterGameModeBase::BeginPlay()
{
	
	HUDWidget = CreateWidget<UHUDUserWidget>(GetGameInstance(), HUDWidgetClass);
	HUDWidget->AddToViewport();
}

void AGlitterGameModeBase::AddNotification(FText content)
{
	
	HUDWidget->PopNotification(content);
}
