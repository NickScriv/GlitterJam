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

	UPROPERTY()
	class UHUDUserWidget* HUDWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UKillScreenUserWidget> killScreenWidgetClass;

	UPROPERTY()
	class UKillScreenUserWidget* KillScreenWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UPauseMenuUserWidget> pauseWidgetClass;

	UPROPERTY()
	class UPauseMenuUserWidget* pauseScreenWidget;

	UPROPERTY(EditAnywhere)
	TArray<class AMonster*> firstScareEventMonster;

	UPROPERTY(EditAnywhere)
		TArray<class ATriggerVolume*> firstScareEventTrigger;

	bool isPaused = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void AddNotification(FText content);

	void EndGame();
	
	UFUNCTION(BlueprintCallable)
	void TogglePause();

	FString queuedMusic = "Play_Ambient_Music";

	UPROPERTY(BlueprintReadOnly)
	float monsterInCaution = 0.f;

	bool isReadingNote = false;
};
