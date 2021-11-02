// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AkSpatialAudioVolume.h"
#include "AkSpatialAudioVolume_Reverb.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API AAkSpatialAudioVolume_Reverb : public AAkSpatialAudioVolume
{
	GENERATED_BODY()

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AAkSpatialAudioVolume_Reverb();

	UPROPERTY(EditAnywhere)
		int32 footStepSurface = 1;

	UPROPERTY(EditAnywhere)
		FString musicName = "Play_Ambient_Music";

	UPROPERTY(EditAnywhere)
	class AStaticMeshActor* lightBlocker;

	UFUNCTION()
	void OnOverlapBegin(class AActor* overlappedActor, class AActor* otherActor);

	
};
