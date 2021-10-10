// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlitterStructs.h"
#include "Safe.generated.h"

UCLASS()
class GLITTERGOLD_API ASafe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASafe();


	UPROPERTY(BlueprintReadOnly)
		bool openDoor;

	UPROPERTY(BlueprintReadWrite)
		bool open;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* mesh;

	UPROPERTY()
	class UInteractionWidgetComponent* interaction;

	UPROPERTY(EditAnywhere, Category = "Safe Values")
	class ALockPick* lockPick;

	UPROPERTY(EditAnywhere)
	float unlockSafeTime;

	bool locked = true;

	bool playerHasPick = false;

	UPROPERTY(EditAnywhere, Category = "MonsterPathing", meta = (EditCondition = locked))
		TArray<FPathPointArray> possibleMonsterPaths;

	UPROPERTY(EditAnywhere)
		class UAkAudioEvent* in_pEventOpen;

	UPROPERTY(EditAnywhere)
		class UAkAudioEvent* in_pEventClose;

	UFUNCTION()
		void InteractSafe(class AMainCharacter* character);

	UFUNCTION()
		void BeginFocusSafe(class AMainCharacter* character);

	UFUNCTION()
		void BeginInteractSafe(class AMainCharacter* character);

	UFUNCTION()
		void EndInteractSafe(class AMainCharacter* character);

	UFUNCTION()
		void PickedUpLockPick();

	UFUNCTION()
		void LockPickUsed();

	void OpenSafe();

	void ChangeMonsterPath();
};
