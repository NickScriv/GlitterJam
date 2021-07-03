// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "TriggerVolume_StairMusicChange.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API ATriggerVolume_StairMusicChange : public ATriggerVolume
{
	GENERATED_BODY()

public:
	ATriggerVolume_StairMusicChange();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnOverlapBegin(class AActor* overlappedActor, class AActor* otherActor);

	UPROPERTY(EditAnywhere)
	int32 rtpcValue = 1;

	UPROPERTY(EditAnywhere)
	ATriggerVolume_StairMusicChange* otherTrigger;
	
};
