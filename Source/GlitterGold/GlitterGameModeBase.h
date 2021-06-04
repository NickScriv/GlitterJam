// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GlitterGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API AGlitterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UHUDUserWidget> HUDWidgetClass;

	class UHUDUserWidget* HUDWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UKillScreenUserWidget> killScreenWidgetClass;

	class UKillScreenUserWidget* KillScreenWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void AddNotification(FText content);

	void EndGame();

	
};
