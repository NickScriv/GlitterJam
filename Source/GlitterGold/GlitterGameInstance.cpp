// Fill out your copyright notice in the Description page of Project Settings.


#include "GlitterGameInstance.h"

UGlitterGameInstance::UGlitterGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{   

}

void UGlitterGameInstance::Init()
{
	Super::Init();

	GEngine->Exec(GetWorld(), TEXT("r.Vsync 0"));
}

void UGlitterGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UGlitterGameInstance::StartGameInstance()
{
	Super::StartGameInstance();
}
