// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"



UENUM()
enum EPathEnding
{
	Nothing    UMETA(DisplayName = "Nothing"),
	PatrolBack   UMETA(DisplayName = "Patrol Back"),
	Loop   UMETA(DisplayName = "Loop")
};

UENUM(BlueprintType)
enum class MonsterStatus : uint8 {
	Patrolling UMETA(DisplayName = "Patrolling"),
	Searching UMETA(DisplayName = "Searching"),
	Chasing UMETA(DisplayName = "Chasing")
};

UCLASS()
class GLITTERGOLD_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<class APathPoint*> path;

	UPROPERTY(EditAnywhere)
		TEnumAsByte <EPathEnding>  pathEnding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 currentPathIndex = 0;

	UPROPERTY(EditAnywhere)
	float killRotTime = 10.f;

	UPROPERTY(BlueprintReadOnly)
	bool killedPlayer = false;

	UPROPERTY(EditAnywhere)
	int32 pathDirection = 1;

	UPROPERTY(BlueprintReadOnly)
	bool startAnimation = false;

	UPROPERTY(EditAnywhere)
	float minDistanceAmbience = 300.f;

	UPROPERTY()
	class AMainCharacter* mainPlayer;
	
	void KillPlayer();

	void StopMonsterSounds();

	void PlayMosnterSoundEvent(FString event);

	int32 passiveEvent = -1;
	int32 cautionEvent = -1;
	int32 chaseEvent = -1;

	UFUNCTION(BlueprintImplementableEvent)
		void TracePath();

protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	FRotator rotateKill;

	UPROPERTY(EditAnywhere)
	class ATriggerVolume* firstEventScare;

	UFUNCTION()
	void TriggerFirstEvent(class AActor* overlappedActor, class AActor* otherActor);

	float ScaleRange(float input, float inputLow, float inputHigh, float outputLow, float outputHigh);

	float ReverseNumber(float num, float min, float max);
};






