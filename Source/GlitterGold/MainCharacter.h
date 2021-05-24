// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

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
		float crawlSpeed = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float crouchSpeed = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float crouchScale = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float crawlScale = 0.5f;

	void MoveForward(float  val);
	void MoveRight(float  val);
	void Turn(float  val);
	void LookUp(float  val);

	void CrouchPressed();
	void CrouchReleased();

#pragma endregion 

};
