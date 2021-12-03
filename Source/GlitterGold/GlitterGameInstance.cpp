// Fill out your copyright notice in the Description page of Project Settings.


#include "GlitterGameInstance.h"
#include "GameFramework/GameUserSettings.h"

UGlitterGameInstance::UGlitterGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{   

}

void UGlitterGameInstance::Init()
{
	Super::Init();

	GEngine->GameUserSettings->SetVSyncEnabled(true);
	GEngine->GameUserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
	GEngine->GameUserSettings->ApplySettings(false);
	GetWorld()->Exec(GetWorld(), TEXT("r.HZBOcclusion 1"));
	//GetWorld()->Exec(GetWorld(), TEXT("t.MaxFPS 60"));
}

void UGlitterGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UGlitterGameInstance::StartGameInstance()
{
	Super::StartGameInstance();
}
