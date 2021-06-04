// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillScreenUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UKillScreenUserWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
		UFUNCTION(BlueprintImplementableEvent)
		void DisplayKillScreen();
	
};
