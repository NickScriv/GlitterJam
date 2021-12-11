// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MonsterAIController.generated.h"

UCLASS()
class GLITTERGOLD_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(BlueprintReadWrite)
		bool isScreaming;

	bool canHear = true;

	//virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	UFUNCTION(BlueprintImplementableEvent, Category = "AICBase")
	void AISightPerceptionViewPoint(FVector& OutLocation, FRotator& OutRotation) const;

	UFUNCTION(BlueprintCallable)
	void StartMonsterBehavior();

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> predictNavClass;

	UPROPERTY()
		AActor* predictNavHelper = nullptr;

	UPROPERTY()
		AActor* playerPredictNavHelper = nullptr;

	bool inSightCone = false;

	void TriggerPatrolAbort();

	FVector GetPredictLoc();

private:

	UPROPERTY()
		class AMainCharacter* player = nullptr;

	UPROPERTY()
		class AMonster* monster = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
		class UBehaviorTree* AIBehavior;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
		class UBlackboardData* AIBlackboard;

	UPROPERTY()
		class UBlackboardComponent* blackboardComp;

	UPROPERTY(EditAnywhere)
		float chaseSpeed = 600.f;

	UPROPERTY(EditAnywhere)
		float ChangePerceptionTime;

	UPROPERTY(EditAnywhere)
		float mainSightRange;

	UPROPERTY(EditAnywhere)
		float sightRange;

	UPROPERTY(EditAnywhere)
		float mainLoseSightRange;

	UPROPERTY(EditAnywhere)
		float loseSightRange;

	UPROPERTY(EditAnywhere)
		float sightHalfAngleDegrees;

	UPROPERTY(EditAnywhere)
		float loseSightHalfAngleDegrees;

	UPROPERTY(EditAnywhere)
		float screamingSightHalfAngleDegrees;

	float currentSightAngle;

	float currentSightRange;

	float currentLoseSightRange;

	float detectRange;

	bool chasing = false;

	UPROPERTY(EditAnywhere)
		float predictionTime;

	UPROPERTY(EditAnywhere, Category = "Components")
	UAIPerceptionComponent* AIPerception = nullptr;

	UFUNCTION()
	void perceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION(BlueprintCallable)
		void StopScreaming();

	void SetToDefaultPerception();

	bool firstSeen = false;

	bool startedBehavior = false;

	bool changePathValue = false;

	bool lostSight = true;

	int32 interchangeValues = 1;

	FTimerHandle timerHandleFirstSeen;

	FVector predictLoc;

	void SetInvestigatingBlackboard();

	bool PlayerLineCheck(bool checkCone);

	void HandleSight(bool successfullySensed);
};
