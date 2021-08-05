// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shotgun.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickedUpShotgun);

UCLASS()
class GLITTERGOLD_API AShotgun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShotgun();

	FOnPickedUpShotgun OnPickedUpShotgun;

	void Shoot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* mesh;

	UPROPERTY(EditDefaultsOnly)
		class UInteractionWidgetComponent* interaction;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* muzzleFlash;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* muzzleSmoke;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* fleshImpact;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* normalImpact;

	UPROPERTY(EditDefaultsOnly)
		int32 pellets;

	UPROPERTY(EditDefaultsOnly)
		float range;

	UPROPERTY(EditDefaultsOnly)
		float coneRadius;

	UPROPERTY(EditDefaultsOnly)
		float hipOffset;

	UPROPERTY(EditDefaultsOnly)
		float aimOffset;


	UFUNCTION()
		void PickedUp(class AMainCharacter* character);

};
