// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flashlight.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickedUpFlashlight);

UCLASS()
class GLITTERGOLD_API AFlashlight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlashlight();

	FOnPickedUpFlashlight OnPickedUpFlashlight;

	void Toggle();

	bool IsFlashlightOn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly)
	class USpotLightComponent* spotLight;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* mesh;

	UPROPERTY(EditDefaultsOnly)
	class UInteractionWidgetComponent* interaction;

	bool flashlightOn = false;

	UFUNCTION()
	void PickedUp(class AMainCharacter* character);

};
