// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarDoorEndGame.generated.h"

UCLASS()
class GLITTERGOLD_API ACarDoorEndGame : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACarDoorEndGame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* box;

	UPROPERTY(EditDefaultsOnly)
		class UInteractionWidgetComponent* interaction;

	UPROPERTY()
	class ULevelSequencePlayer* SequencePlayer;

	UPROPERTY(EditAnywhere)
		class ALevelSequenceActor* SequenceActor;

	UPROPERTY(EditAnywhere)
	class ULevelSequence* SequenceGoodEnding;

	UPROPERTY(EditAnywhere)
	class AGlitterGameModeBase* gameMode;

	FTimerHandle goodEndCutsceneHandle;

	UFUNCTION()
		void EndGame(class AMainCharacter* character);

	UFUNCTION()
		void ToMainMenu();

	void TriggerGoodEnding();

};
