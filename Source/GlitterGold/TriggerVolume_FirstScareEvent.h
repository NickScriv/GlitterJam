// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "TriggerVolume_FirstScareEvent.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API ATriggerVolume_FirstScareEvent : public ATriggerVolume
{
	GENERATED_BODY()

	public:
		ATriggerVolume_FirstScareEvent();

	protected:
	virtual void BeginPlay() override;

	private:
	UFUNCTION()
	void OnOverlapBegin(class AActor* overlappedActor, class AActor* otherActor);
	
	
};
