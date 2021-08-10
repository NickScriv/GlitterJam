// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LockPick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickedUpLockPick);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLockPickUsed);

UCLASS()
class GLITTERGOLD_API ALockPick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALockPick();

	FOnPickedUpLockPick onPickedUpLockPick;
	FOnLockPickUsed onLockPickUsed;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* lockPickMesh;

	UPROPERTY(EditDefaultsOnly)
		class UInteractionWidgetComponent* interaction;

	UFUNCTION()
		void PickedUp(class AMainCharacter* character);

};
