// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "MainCharacter.generated.h"


class UCurveFloat;

UENUM()
enum EMovement
{
	Standing    UMETA(DisplayName = "Standing"),
	Crouching   UMETA(DisplayName = "Crouching")
};


USTRUCT()
struct FInteractionData
{
	GENERATED_BODY()

	FInteractionData()
	{
		viewedInteractionItem = nullptr;
		lastInteractCheckTime = 0.f;
		interactKeyHeld = false;
	}

	UPROPERTY()
		class UInteractionWidgetComponent* viewedInteractionItem;

	UPROPERTY()
		float lastInteractCheckTime;

	UPROPERTY()
		bool interactKeyHeld;

};

UCLASS()
class GLITTERGOLD_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsInteracting() const;

	float GetRemainingInteractTime();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float stamina = 100.f;

	UPROPERTY(EditAnywhere, Category = "Death")
		float deathRotTime = 5.f;

	UPROPERTY(EditAnywhere, Category = "Death")
		float monsterOffsetLookAt = 100.f;

	UPROPERTY(EditAnywhere, Category = "Death")
		float monsterOffsetLookAtCrouch = 50.f;

	UFUNCTION()
	void TimelineProgressCrouch(float val);

	void NotTest();

	void Died(class AMonster* monster);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region Interaction
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
		float interactionCheckDistance = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Interaction")
		float interactionCheckRadius = 5.f;

	FTimerHandle timerHandleInteract;

	UPROPERTY()
		FInteractionData interactionData;

	void PerformCheck();

	void NoInteractableFound();
	void FoundNewInteractable(UInteractionWidgetComponent* interactable);
	void BeginInteraction();
	void EndInteraction();


	void Interact();

#pragma endregion

private:	

	bool died = false;
	FRotator rotateDeath;

	UPROPERTY(EditDefaultsOnly)
		class UCameraComponent* cameraComponent;

#pragma region Movement
	UPROPERTY(EditAnywhere, Category = "Movement")
		float walkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float sprintSpeed = 900.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float crouchSpeed = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float crouchScale = 0.5f;

	UPROPERTY(EditAnywhere)
		float sphereCastRange = 125.f;

	UPROPERTY(EditAnywhere)
		float footStepTimeWalk = .5f;

	UPROPERTY(EditAnywhere)
		float footStepTimeSprint = .3f;

	UPROPERTY(EditAnywhere)
		float footStepTimeCrouch = .8f;

	UPROPERTY(EditAnywhere)
		float crouchLoudness = .4f;

	UPROPERTY(EditAnywhere)
		float walkLoudness = .8f;

	UPROPERTY(EditAnywhere)
		float sprintLoudness = 1.5f;

	UPROPERTY(EditAnywhere)
		float heavyBreathThreshold = 40.f;

	UPROPERTY(EditAnywhere)
		class USoundBase* footSound;

	FTimerHandle timerFootstep;
	bool playFootStep = true;

	UFUNCTION()
	void PlayFootStep();

	void ProcessFootStep();

	bool IsSprinting();

	float crouchCameraHeight;
	float standCameraHeight;
	float standCapsuleHeight;
	float crouchCapsuleHeight;
	bool stuckOnCrouch = false;

	bool isCrouchHeld = false;

	bool isCrouching = false;


	bool canSprint = true;

	int32 heavySprintEvent = -1;
	int32 gentleSprintEvent = -1;
	int32 depletedSprintEvent = -1;
	int32 startSprintEvent = -1;
	
	FTimerDelegate sprintKeyDelayCallback;
	FTimerHandle sprintKeyDelay;
	FTimerHandle timerStamina;

	FTimeline crouchCurveTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* crouchCurveFloat;

	void MoveForward(float  val);
	void MoveRight(float  val);
	void Turn(float  val);
	void LookUp(float  val);
	void CrouchPressed();
	void CrouchReleased();
	void BeginCrouch();
	void EndCrouch();
	void SprintPressed();
	void SprintReleased();
	void SetMovement(EMovement newMovement);
	void RegainStamina();
	float CalculateRestTime(float oldMin, float oldMax, float newMin, float newMax);


	UPROPERTY(EditAnywhere, Category = "Movement")
		float staminaRegen = 2;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float staminaDepletion = 3;


	bool isSprintingKeyDown = false;
	bool isCrouchingKeyDown = false;
	bool canPressSprint = true;
	TEnumAsByte <EMovement> movement = EMovement::Standing;
	class UCapsuleComponent* capsuleColl;


	UFUNCTION()
		void ResolveMovement();

	bool AreStaminaEventsPlaying();



#pragma endregion 

};
