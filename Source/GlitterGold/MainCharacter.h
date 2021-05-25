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

	UFUNCTION()
	void TimelineProgressCrouch(float val);

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


	
	float crouchCameraHeight;
	float standCameraHeight;
	float standCapsuleHeight;
	float crouchCapsuleHeight;

	bool isCrouchHeld = false;

	bool isCrouching = false;

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

	UPROPERTY(EditAnywhere, Category = "Movement")
		float staminaRegen = 2;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float staminaDepletion = 3;


	bool isSprintingKeyDown = false;
	bool isCrouchingKeyDown = false;
	TEnumAsByte <EMovement> movement = EMovement::Standing;
	class UCapsuleComponent* capsuleColl;


	UFUNCTION()
		void ResolveMovement();



#pragma endregion 

};
