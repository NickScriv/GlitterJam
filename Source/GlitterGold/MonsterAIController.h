// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MonsterAIController.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActorPerceptionUpdatedDelegate, AActor*, actor, FAIStimulus, stimulus);

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

public:
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	UFUNCTION(BlueprintImplementableEvent, Category = "AICBase")
	void AISightPerceptionViewPoint(FVector& OutLocation, FRotator& OutRotation) const;

	UFUNCTION(BlueprintCallable)
	void StartMonsterBehavior();

	void TriggerPatrolAbort();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
		class UBehaviorTree* AIBehavior;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
		class UBlackboardData* AIBlackboard;

	UPROPERTY()
		class UBlackboardComponent* blackboardComp;

	UPROPERTY(EditAnywhere)
		float chaseSpeed = 600.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	TSubclassOf<UAISense> hearingSense;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
	TSubclassOf<UAISense> sightSense;

	UPROPERTY(EditAnywhere, Category = "Components")
	UAIPerceptionComponent* AIPerceptionStart = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	UAIPerceptionComponent* AIPerception = nullptr;

	UFUNCTION()
	void perceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	bool firstSeen = false;

	bool changePathValue = false;

	FTimerHandle timerHandleFirstSeen;

	void SetToDefaultPerception();
};
