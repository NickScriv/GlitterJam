// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void PopNotification(const FText& content);

	UFUNCTION(BlueprintImplementableEvent)
	void ReticleUI(bool show);

	UFUNCTION(BlueprintImplementableEvent)
	void AmmoUI(bool show, int32 ammoCount);

	UFUNCTION(BlueprintImplementableEvent)
	void FadeInHUD();

	UFUNCTION(BlueprintImplementableEvent)
	void FadeOutHUD();
};
