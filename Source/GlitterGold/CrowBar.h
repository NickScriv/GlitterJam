// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrowBar.generated.h"


UCLASS()
class GLITTERGOLD_API ACrowBar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrowBar();

	void SwingAttack();

	void GetPlayer(class AMainCharacter* character);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* crowBarMesh;

	UPROPERTY(EditDefaultsOnly)
		class UInteractionWidgetComponent* interaction;

	/*UPROPERTY()
		class UBoxComponent* hitBox;

	UPROPERTY()
		class AMainCharacter* player;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* normalHit;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* monsterHit;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* woodHit;*/

	UFUNCTION()
		void PickedUp(class AMainCharacter* character);

	/*UFUNCTION()
		void HitSomething(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/
};
