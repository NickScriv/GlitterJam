// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "TriggerVolume_LightShort.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API ATriggerVolume_LightShort : public ATriggerVolume
{
	GENERATED_BODY()

	bool playerEntered = false;

	UFUNCTION()
		void OnOverlapBegin(class AActor* overlappedActor, class AActor* otherActor);

	UPROPERTY(EditAnywhere)
		TArray<class ACeilingLight*> lights;

	FOnAkPostEventCallback turnOffLights;

	UPROPERTY(EditAnywhere)
		class UAkAudioEvent* in_pEventLightsOff;

	UPROPERTY(EditAnywhere)
		float lightFlickerTime;

	UFUNCTION()
		void TurnOffLights();

	FTimerHandle lightShortTimer;

protected:
	virtual void BeginPlay() override;
	
};
