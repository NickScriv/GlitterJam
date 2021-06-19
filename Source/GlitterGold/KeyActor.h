// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickedUpKey);

UCLASS()
class GLITTERGOLD_API AKeyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeyActor();

	FOnPickedUpKey OnPickedUpKey;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* keyMesh;

	UPROPERTY(EditDefaultsOnly)
	class UInteractionWidgetComponent* interaction;

	UFUNCTION()
	void PickedUp(class AMainCharacter* character);

	
};
