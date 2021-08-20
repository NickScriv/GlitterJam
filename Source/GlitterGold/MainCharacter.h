// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "MainCharacter.generated.h"


class AFlashlight;
class ACrowBar;
class AShotgun;
class AMainCollectible;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float walkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float sprintSpeed = 900.0f;

	UPROPERTY(BlueprintReadWrite)
	bool isShooting = false;

	UPROPERTY(BlueprintReadWrite)
	bool isSwinging = false;

	UPROPERTY(BlueprintReadWrite)
	bool isAiming = false;

	UPROPERTY(BlueprintReadWrite)
	int32 prevHandSlot;

	UPROPERTY(BlueprintReadWrite)
	int32 currentHandSlot = 0;

	UPROPERTY(BlueprintReadWrite)
	bool hideArmsFlashlight = true;

	UPROPERTY(BlueprintReadWrite)
		bool hideArmsShotgun = true;

	UPROPERTY(BlueprintReadWrite)
		bool hideArmsCrowBar = true;

	UPROPERTY(BlueprintReadWrite)
	bool canSwitch = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class AMainCollectible* syringe = nullptr;

	int32 numberOfKeys = 0;

	int32 shotgunBulletCount = 1;

	FTimerHandle timerHandleShoot;

	bool axeCanDamage = false;

	void PickedUpKey();

	void StopPlayerSounds();

	UFUNCTION()
	void TimelineProgressCrouch(float val);

	void SpawnFlashlight();

	void SpawnShotgun();

	void SpawnCrowBar();

	void InjectSyringe();

	void NotTest();

	void Died(class AMonster* monster);
	
	bool IsSprinting();

	void HideFlashlight();

	void HideShotgun();

	void HideCrowBar();

	void ShowFlashlight();

	void ShowShotgun();

	void ShowCrowBar();

	void Attack();

	void AimPressed();

	void AimReleased();

	void GetCrossHairScreenCoordinates(FVector& pos, FVector& dir);

	FTransform GetCameraTransform();

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

	UPROPERTY()
	class AGlitterGameModeBase* gameMode;

	bool died = false;
	FRotator rotateDeath;

	UPROPERTY(EditDefaultsOnly)
		class UCameraComponent* cameraComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFlashlight> flashlightClass;

	UPROPERTY(EditDefaultsOnly)
	class AFlashlight* flashlight = nullptr;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AShotgun> shotgunClass;

	UPROPERTY(EditDefaultsOnly)
		class AShotgun* shotgun = nullptr;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ACrowBar> crowBarClass;

	UPROPERTY(EditDefaultsOnly)
		class ACrowBar* crowBar = nullptr;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AMainCollectible> syringeClass;

	TArray<bool> items;


#pragma region Movement


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
		float maxNormalRestTime = 3.f;

	UPROPERTY(EditAnywhere)
		float depletedRestTime = 3.5f;

	UPROPERTY(EditAnywhere)
		float heavyBreathThreshold = 40.f;


	FTimerHandle timerFootstep;
	bool playFootStep = true;

	UFUNCTION()
	void PlayFootStep();

	void ProcessFootStep();

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

	FTimeline crouchCurveTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* crouchCurveFloat;

	UPROPERTY(EditAnywhere, Category = "Stamina")
		UCurveFloat* restTimeCurveFloat;

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

	void SwitchArms(float val);

	UPROPERTY(EditAnywhere, Category = "Stamina")
		float staminaRegen = 2;

	UPROPERTY(EditAnywhere, Category = "Stamina")
		float staminaDepletion = 3;


	bool isSprintingKeyDown = false;
	bool isCrouchingKeyDown = false;
	bool canPressSprint = true;
	TEnumAsByte <EMovement> movement = EMovement::Standing;

	UPROPERTY()
	class UCapsuleComponent* capsuleColl;

	UFUNCTION()
		void ResolveMovement();

	bool AreStaminaEventsPlaying();

	float restTimer = 0.2f;
	float restCountDown = 0.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float crouchDelayTimer = 0.35f;
	
	float crouchDelayCountdown = 0.f;
	bool canCrouch = true;

	float footTimer = walkSpeed;

	float footCountdown = 0.f;

	bool canStep = false;

#pragma endregion

	void FlashlightToggle();

	void TogglePause();

	int32 GetTargetHandSlot(float val);

	void WrapNumberWithinRange(int32& num, int32 min, int32 max);

	void AimIn();
	void AimOut();
};
