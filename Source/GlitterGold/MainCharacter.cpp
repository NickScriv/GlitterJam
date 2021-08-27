// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractionWidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GlitterGameModeBase.h"
#include "Perception/AISense_Hearing.h"
#include "Monster.h"
#include "Flashlight.h"
#include "CrowBar.h"
#include "Shotgun.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainCollectible.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkGameplayStatics.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"
#include "GlitterGameInstance.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleColl = GetCapsuleComponent();
	cameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	cameraComponent->SetupAttachment(capsuleColl);
	cameraComponent->bUsePawnControlRotation = true;

	GetMesh()->SetupAttachment(cameraComponent);

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->AttachToComponent(cameraComponent, FAttachmentTransformRules::KeepWorldTransform);
	UGlitterGameInstance* gameInstance = Cast<UGlitterGameInstance>(UGameplayStatics::GetGameInstance(this));

	

	// Make character shorter for ending
	if (gameInstance && gameInstance->gameEnding)
	{
		FVector camLoc = cameraComponent->GetRelativeLocation();
		camLoc.Z = camLoc.Z * scaleHeightEnding;
		cameraComponent->SetRelativeLocation(camLoc);

		float height = capsuleColl->GetScaledCapsuleHalfHeight() * scaleHeightEnding;
		capsuleColl->SetCapsuleHalfHeight(height);

		gameInstance->monsterKilled = true;
	}
	
	standCameraHeight = cameraComponent->GetRelativeLocation().Z;
	crouchCameraHeight = standCameraHeight * crouchScale;
	standCapsuleHeight = capsuleColl->GetScaledCapsuleHalfHeight();
	crouchCapsuleHeight = capsuleColl->GetScaledCapsuleHalfHeight() * crouchScale;

	capsuleColl->SetCollisionObjectType(ECC_GameTraceChannel3);
	capsuleColl->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	items.Add(true);
	items.Add(false);
	items.Add(false);
	items.Add(false);

	if (crouchCurveFloat)
	{
	
		FOnTimelineFloat timelineProgress;
		timelineProgress.BindUFunction(this, FName("TimelineProgressCrouch"));
		crouchCurveTimeline.AddInterpFloat(crouchCurveFloat, timelineProgress);
		crouchCurveTimeline.SetLooping(false);
	}

	if (!restTimeCurveFloat)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rest time curve not intialized!"));
		
	}

	GetWorldTimerManager().ClearTimer(timerHandleInteract);

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	
	FAkAudioDevice::Get()->SetRTPCValue(*FString("Footsteps_Movement_Type"), 2, 200, this);
	FAkAudioDevice::Get()->SetRTPCValue(*FString("Downstairs_Upstairs_Stairwell"), 1, 300, this);

	FAkAudioDevice::Get()->PostEvent("PLAY_MUSIC", this);
	FAkAudioDevice::Get()->PostEvent("Play_Ambient_Music", this);
	FAkAudioDevice::Get()->SetRTPCValue(*FString("Num_of_Keys"), 0, 300, this);

	gameMode = Cast<AGlitterGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (!gameMode)
		return;

	gameMode->ReticleUI(false);
	gameMode->AmmoUI(false, shotgunBulletCount);
	gameMode->FadeInHUD();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult OutHit;
	GetCharacterMovement()->SafeMoveUpdatedComponent(FVector(0.f, 0.f, 0.01f), GetActorRotation(), true, OutHit);
	GetCharacterMovement()->SafeMoveUpdatedComponent(FVector(0.f, 0.f, -0.01f), GetActorRotation(), true, OutHit);

	PerformCheck();

	
	crouchCurveTimeline.TickTimeline(DeltaTime);

	restCountDown += DeltaTime;
	if(restCountDown >= restTimer)
	{
		restCountDown = FMath::Clamp(restCountDown, 0.f, restTimer);
		canSprint = true;
	}

	crouchDelayCountdown += DeltaTime;
	if (crouchDelayCountdown >= crouchDelayTimer)
	{
		crouchDelayCountdown = FMath::Clamp(crouchDelayCountdown, 0.f, crouchDelayTimer);
		canCrouch = true;
	}

	if (died)
	{
		FRotator res = FMath::RInterpTo(cameraComponent->GetComponentRotation(), rotateDeath, DeltaTime, deathRotTime);

		cameraComponent->SetWorldRotation(res);
	}

	if (GetVelocity().Size() > walkSpeed + 5.f)
	{
		stamina -= staminaDepletion * DeltaTime;
		stamina = FMath::Clamp(stamina, 0.f, 100.f);
	}
	else 
	{
		stamina += staminaRegen * DeltaTime;
		stamina = FMath::Clamp(stamina, 0.f, 100.f);

		/*if(movement == EMovement::Standing)
		{
			GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = crouchSpeed;
		}*/
	}

	if (stamina < 1.f && movement == EMovement::Standing)
	{
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		FAkAudioDevice::Get()->SetRTPCValue(*FString("Footsteps_Movement_Type"), 2, 200, this);
		depletedSprintEvent = FAkAudioDevice::Get()->PostEvent("Stamina_Depleted", this);
		stamina = 1.f;
		canSprint = false;
		restTimer = depletedRestTime;
		restCountDown = 0.f;
	}

	FVector top = GetActorLocation();
	top.Z += capsuleColl->GetScaledCapsuleHalfHeight();
	FVector endTrace = top + (GetActorUpVector() * sphereCastRange);
	TArray<AActor*> actorsToIgnore;

	actorsToIgnore.Add(this);
	FHitResult hit;

	if (stuckOnCrouch && movement == EMovement::Crouching && !UKismetSystemLibrary::SphereTraceSingle(this, top, endTrace, capsuleColl->GetScaledCapsuleRadius(), UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore, EDrawDebugTrace::None, hit, true))
	{
		stuckOnCrouch = false;
		SetMovement(EMovement::Standing);
	}

	ProcessFootStep();

	if(canStep)
	{
		footCountdown += DeltaTime;

		if (footCountdown >= footTimer)
		{
			footCountdown = FMath::Clamp(crouchDelayCountdown, 0.f, footTimer);
			footCountdown = 0.0f;
			PlayFootStep();
		}
	}
	

	FAkAudioDevice::Get()->SetRTPCValue(*FString("Stamina_Level"), stamina, 500, this);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainCharacter::BeginInteraction);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &AMainCharacter::EndInteraction);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::CrouchPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMainCharacter::CrouchReleased);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::SprintPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::SprintReleased);

	PlayerInputComponent->BindAxis("SwitchArms", this, &AMainCharacter::SwitchArms);

	PlayerInputComponent->BindAction("Flashlight", IE_Pressed, this, &AMainCharacter::FlashlightToggle);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AMainCharacter::TogglePause).bExecuteWhenPaused = true;

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AMainCharacter::Attack);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMainCharacter::AimPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMainCharacter::AimReleased);
	//PlayerInputComponent->BindAction("TestNot", IE_Pressed, this, &AMainCharacter::NotTest);

}

void AMainCharacter::SpawnFlashlight()
{
	flashlight = GetWorld()->SpawnActor<AFlashlight>(flashlightClass);
	flashlight->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Flashlight"));
	flashlight->SetActorHiddenInGame(true);
	flashlight->SetOwner(this);
	items[1] = true;
}

void AMainCharacter::SpawnShotgun()
{
	shotgun = GetWorld()->SpawnActor<AShotgun>(shotgunClass);
	shotgun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Shotgun"));
	shotgun->SetActorHiddenInGame(true);
	shotgun->SetOwner(this);
	items[2] = true;
}

void AMainCharacter::SpawnCrowBar()
{
	crowBar = GetWorld()->SpawnActor<ACrowBar>(crowBarClass);
	crowBar->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("CrowBar"));
	crowBar->SetActorHiddenInGame(true);
	crowBar->SetOwner(this);
	//crowBar->GetPlayer(this);
	items[3] = true;
}

void AMainCharacter::InjectSyringe()
{
	canSwitch = false;
	hideArmsFlashlight = true;
	hideArmsShotgun = true;
	hideArmsCrowBar = true;
	prevHandSlot = currentHandSlot;
	currentHandSlot = 4;
	syringe = GetWorld()->SpawnActor<AMainCollectible>(syringeClass);
	syringe->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Syringe"));
	syringe->SetActorHiddenInGame(true);

}

void AMainCharacter::NotTest()
{
	if (gameMode)
		gameMode->AddNotification(FText::FromString("Notification!"));
}

void AMainCharacter::Died(AMonster* monster)
{
	if ((movement == EMovement::Crouching))
	{
		monsterOffsetLookAt = 0;
	}

	FVector monsterLookAt = monster->GetActorLocation();
	monsterLookAt = FVector(monsterLookAt.X, monsterLookAt.Y, monsterLookAt.Z + monsterOffsetLookAt);
	rotateDeath = UKismetMathLibrary::FindLookAtRotation(cameraComponent->GetComponentLocation(), monsterLookAt);
	DisablePlayer();
	died = true;
}

void AMainCharacter::DiedEnding(FVector lookAtLoc)
{
	if ((movement == EMovement::Crouching))
	{
		monsterOffsetLookAt = 0;
	}

	
	FVector monsterLookAt = lookAtLoc;
	monsterLookAt = FVector(monsterLookAt.X, monsterLookAt.Y, monsterLookAt.Z + monsterOffsetLookAt);
	rotateDeath = UKismetMathLibrary::FindLookAtRotation(cameraComponent->GetComponentLocation(), monsterLookAt);
	DisablePlayer();
	died = true;

}

void AMainCharacter::DisablePlayer()
{
	SetMovement(EMovement::Standing);
	DisableInput(UGameplayStatics::GetPlayerController(this, 0));
	cameraComponent->bUsePawnControlRotation = false;
	GetCharacterMovement()->DisableMovement();
}

#pragma region Interaction

void AMainCharacter::PerformCheck()
{

	if (!GetController())
		return;

	interactionData.lastInteractCheckTime = GetWorld()->GetTimeSeconds();

	FVector eyesLoc;
	FRotator eyesRot;
	GetController()->GetPlayerViewPoint(eyesLoc, eyesRot);

	FVector endTrace = (eyesRot.Vector() * interactionCheckDistance) + eyesLoc;

	TArray<AActor*> actorsToIgnore;

	actorsToIgnore.Add(this);
	FHitResult hit;

	if (UKismetSystemLibrary::SphereTraceSingle(this, eyesLoc, endTrace, interactionCheckRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore, EDrawDebugTrace::None, hit, true))
	{
		

		if (hit.GetActor())
		{
			if (UInteractionWidgetComponent* interactionComponent = Cast<UInteractionWidgetComponent>(hit.GetActor()->GetComponentByClass(UInteractionWidgetComponent::StaticClass())))
			{
				
				float dist = (eyesLoc - hit.ImpactPoint).Size();

				if (interactionComponent != interactionData.viewedInteractionItem && dist <= interactionComponent->interactionDistance)
				{
					
					FoundNewInteractable(interactionComponent);
				}
				else if (dist > interactionComponent->interactionDistance)
				{
					
					NoInteractableFound();
				}

				return;
			}
		}
	}

	NoInteractableFound();
}

void AMainCharacter::NoInteractableFound()
{

	if (GetWorldTimerManager().IsTimerActive(timerHandleInteract))
	{
		GetWorldTimerManager().ClearTimer(timerHandleInteract);
	}

	if (UInteractionWidgetComponent* oldInteractable = interactionData.viewedInteractionItem)
	{
		oldInteractable->EndFocus();

		if (interactionData.interactKeyHeld)
		{
			EndInteraction();
		}
	}

	interactionData.viewedInteractionItem = nullptr;
}

void AMainCharacter::FoundNewInteractable(UInteractionWidgetComponent* interactable)
{
	EndInteraction();

	if (UInteractionWidgetComponent* oldInteractable = interactionData.viewedInteractionItem)
	{
		oldInteractable->EndFocus();
	}
	interactionData.viewedInteractionItem = interactable;
	interactable->BeginFocus();
}

void AMainCharacter::BeginInteraction()
{
	interactionData.interactKeyHeld = true;
	
	if (UInteractionWidgetComponent* interactable = interactionData.viewedInteractionItem)
	{
		interactable->BeginInteraction();

		if (FMath::IsNearlyZero(interactable->interactionTime))
		{
			Interact();
		}
		else
		{
			GetWorldTimerManager().SetTimer(timerHandleInteract, this, &AMainCharacter::Interact, interactable->interactionTime, false);
		}
	}
}

void AMainCharacter::EndInteraction()
{
	interactionData.interactKeyHeld = false;

	GetWorldTimerManager().ClearTimer(timerHandleInteract);

	if (UInteractionWidgetComponent* interactable = interactionData.viewedInteractionItem)
	{
		interactable->EndInteraction();
	}
}

void AMainCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interact"));
	GetWorldTimerManager().ClearTimer(timerHandleInteract);

	if (UInteractionWidgetComponent* interactable = interactionData.viewedInteractionItem)
	{
		interactable->Interact();
	}
}

bool AMainCharacter::IsInteracting() const
{
	return GetWorldTimerManager().IsTimerActive(timerHandleInteract);
}

float AMainCharacter::GetRemainingInteractTime()
{
	return GetWorldTimerManager().GetTimerRemaining(timerHandleInteract);
}



#pragma endregion 

#pragma region Movement

void AMainCharacter::TimelineProgressCrouch(float val)
{
	//UE_LOG(LogTemp, Warning, TEXT("crouch"));
	FVector camLoc = cameraComponent->GetRelativeLocation();
	camLoc.Z = FMath::Lerp(standCameraHeight, crouchCameraHeight, val);
	cameraComponent->SetRelativeLocation(camLoc);

	float height = FMath::Lerp(standCapsuleHeight, crouchCapsuleHeight, val);
	capsuleColl->SetCapsuleHalfHeight(height);
}


void AMainCharacter::PlayFootStep()
{
	FAkAudioDevice::Get()->PostEvent("Play_Footstep", this);
	//TODO: Remember to change these values back to crouchLoudness, walkLoudness, and sprintLoudness instead of 0.0
	if (EMovement::Crouching == movement)
	{
		
		UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), 0.0f, this, 0, FName("Noise"));
	}
	else if(IsSprinting())
	{
		UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), 0.0f, this, 0, FName("Noise"));
	}
	else
	{
		UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), 0.0f, this, 0, FName("Noise"));
	}
}

void AMainCharacter::ProcessFootStep()
{
	FVector vel = GetVelocity();
	float speed = vel.Size();

	if (speed >= 1.f)
	{
		canStep = true;
		float footTime;

		if (movement == EMovement::Crouching)
		{
			footTime = footStepTimeCrouch;
		}
		else if (speed > walkSpeed + 5.f)
		{
			footTime = footStepTimeSprint;
		}
		else
		{
			footTime = footStepTimeWalk;
		}
		
		/*GetWorldTimerManager().SetTimer(timerFootstep, this, &AMainCharacter::PlayFootStep, footTime, true);
		playFootStep = false;*/

		footTimer = footTime;
	}
	else
	{
		GetWorldTimerManager().ClearTimer(timerFootstep);
		canStep = false;
	}
}

bool AMainCharacter::IsSprinting()
{
	return stamina > 1.f && isSprintingKeyDown && canSprint;
}

void AMainCharacter::HideFlashlight()
{
	if(flashlight)
		flashlight->SetActorHiddenInGame(true);
}

void AMainCharacter::HideShotgun()
{
	if(shotgun)
		shotgun->SetActorHiddenInGame(true);
}

void AMainCharacter::HideCrowBar()
{
	if (crowBar)
		crowBar->SetActorHiddenInGame(true);
}

void AMainCharacter::ShowFlashlight()
{
	if (flashlight)
		flashlight->SetActorHiddenInGame(false);
}

void AMainCharacter::ShowShotgun()
{
	if(shotgun)
		shotgun->SetActorHiddenInGame(false);
}

void AMainCharacter::ShowCrowBar()
{
	if (crowBar)
		crowBar->SetActorHiddenInGame(false);
}

void AMainCharacter::Attack()
{
	if (canSwitch && GetMovementComponent()->Velocity.Size() <= walkSpeed + 0.05f)
	{
		if (shotgun && currentHandSlot == 2 && !isShooting)
		{

			if (gameMode && gameMode->playerKilled)
			{
				return;
			}

			if (shotgunBulletCount > 0)
			{
				// shoot
				FAkAudioDevice::Get()->PostEvent("Shotgun_Fire", this);
				isShooting = true;
				shotgun->Shoot();

				shotgunBulletCount--;
				gameMode->AmmoUI(true, shotgunBulletCount);

			}
			else
			{
				FAkAudioDevice::Get()->PostEvent("Shotgun_Empty", this);
			}
		}
		else if (crowBar && currentHandSlot == 3 && !isSwinging)
		{
			isSwinging = true;
			crowBar->SwingAttack();
		}
	}
	
}

void AMainCharacter::AimPressed()
{
	if (shotgun && currentHandSlot == 2 && canSwitch && GetMovementComponent()->Velocity.Size() <= walkSpeed + 0.05f && !isShooting && !isAiming)
	{
		if (gameMode && !gameMode->playerKilled)
		{
			AimIn();
		}
		
	}
}

void AMainCharacter::AimReleased()
{
	if (isAiming)
	{
		AimOut();
	}
}

void AMainCharacter::GetCrossHairScreenCoordinates(FVector& pos, FVector& dir)
{
	APlayerController* playerController = Cast<APlayerController>(GetController());

	int32 x, y;
	if (playerController)
	{
		playerController->GetViewportSize(x, y);
		playerController->DeprojectScreenPositionToWorld(x * 0.5, y * 0.58035, pos, dir);
	}
}

FTransform AMainCharacter::GetCameraTransform()
{
	return cameraComponent->GetComponentTransform();
}

void AMainCharacter::MoveForward(float val)
{
	AddMovementInput(GetActorForwardVector(), val);
}

void AMainCharacter::MoveRight(float val)
{
	AddMovementInput(GetActorRightVector(), val);
}

void AMainCharacter::Turn(float val)
{
	AddControllerYawInput(val);
}

void AMainCharacter::LookUp(float val)
{
	AddControllerPitchInput(val);
}


void AMainCharacter::CrouchPressed()
{
	isCrouchingKeyDown = true;

	if (movement != EMovement::Standing || GetCharacterMovement()->IsFalling() || !canCrouch)
		return;

	canCrouch = false;
	crouchDelayCountdown = 0.f;
	
	if(IsSprinting() && stamina > 1.f)
	{
		if (stamina < heavyBreathThreshold )
		{
			heavySprintEvent = FAkAudioDevice::Get()->PostEvent("Stop_Run_Heavy", this);
			
		}
		else 
		{

			gentleSprintEvent = FAkAudioDevice::Get()->PostEvent("Stop_Run_Gentle", this);
		}

		float restTime = restTimeCurveFloat->GetFloatValue(stamina);
		UE_LOG(LogTemp, Warning, TEXT("%f"), restTime);

		canSprint = false;
		restTimer = restTime;
		restCountDown = 0.f;
	}
	
	

	//crouch
	SetMovement(EMovement::Crouching);
	

}

void AMainCharacter::CrouchReleased()
{
	isCrouchingKeyDown = false;

	if (movement != EMovement::Crouching || GetCharacterMovement()->IsFalling())
		return;

	FVector top = GetActorLocation();
	top.Z += capsuleColl->GetScaledCapsuleHalfHeight();
	FVector endTrace = top + (GetActorUpVector() * sphereCastRange);
	TArray<AActor*> actorsToIgnore;

	actorsToIgnore.Add(this);
	FHitResult hit;
	if (UKismetSystemLibrary::SphereTraceSingle(this, top, endTrace, capsuleColl->GetScaledCapsuleRadius(), UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore, EDrawDebugTrace::None, hit, true))
	{
		stuckOnCrouch = true;
		return;
	}

	SetMovement(EMovement::Standing);


}

void AMainCharacter::SprintPressed()
{
	if (movement != EMovement::Standing || GetCharacterMovement()->IsFalling())
		return;

	isSprintingKeyDown = true;
	
	if(canSprint && !isAiming && !isShooting && !isSwinging)
	{
		UE_LOG(LogTemp, Warning, TEXT("can sprint"));
		GetCharacterMovement()->MaxWalkSpeed = sprintSpeed;
		FAkAudioDevice::Get()->SetRTPCValue(*FString("Footsteps_Movement_Type"), 3, 200, this);
		startSprintEvent = FAkAudioDevice::Get()->PostEvent("Start_Run", this);
	}
		
}

void AMainCharacter::SprintReleased()
{
	if(IsSprinting() && stamina >= 1.f)
	{
		if (stamina < heavyBreathThreshold )
		{

			heavySprintEvent = FAkAudioDevice::Get()->PostEvent("Stop_Run_Heavy", this);
		}
		else 
		{

			gentleSprintEvent = FAkAudioDevice::Get()->PostEvent("Stop_Run_Gentle", this);
		}

		float restTime = restTimeCurveFloat->GetFloatValue(stamina);

		UE_LOG(LogTemp, Warning, TEXT("%f"), restTime);
		canSprint = false;
		restTimer = restTime;
		restCountDown = 0.f;
	}
	
	
	isSprintingKeyDown = false;

	
	
	if (movement != EMovement::Standing || GetCharacterMovement()->IsFalling())
		return;

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	FAkAudioDevice::Get()->SetRTPCValue(*FString("Footsteps_Movement_Type"), 2, 200, this);
	//GetWorldTimerManager().SetTimer(timerFootstep, this, &AMainCharacter::PlayFootStep, footStepTimeWalk, true);

	
}

void AMainCharacter::BeginCrouch()
{
	// crouch

	crouchCurveTimeline.Play();
	GetCharacterMovement()->MaxWalkSpeed = crouchSpeed;
	FAkAudioDevice::Get()->SetRTPCValue(*FString("Footsteps_Movement_Type"), 1, 200, this);
	//GetWorldTimerManager().SetTimer(timerFootstep, this, &AMainCharacter::PlayFootStep, footStepTimeCrouch, true);
	
	
}

void AMainCharacter::EndCrouch()
{
	// stand
	crouchCurveTimeline.Reverse();

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	FAkAudioDevice::Get()->SetRTPCValue(*FString("Footsteps_Movement_Type"), 2, 200, this);
}


void AMainCharacter::SetMovement(EMovement newMovement)
{
	movement = newMovement;

	switch (newMovement)
	{
	case EMovement::Crouching:
		BeginCrouch();
		break;
	case EMovement::Standing:
		EndCrouch();
		break;
	}
}

void AMainCharacter::RegainStamina()
{
	canSprint = true;
}

//TODO: Remember to change assignment to canSwitch and isShooting and isAiming!!!
void AMainCharacter::SwitchArms(float val)
{
	if(!FMath::IsNearlyZero(val) && canSwitch  && !isSwinging)
	{	
		int32 targetHandSlot = GetTargetHandSlot(val);

		if (targetHandSlot == currentHandSlot)
			return;

		bool showBulletCountUI = false;
		bool showCrossHairUI = false;
		
		if (targetHandSlot == 0)
		{
			// Show nothing
			hideArmsFlashlight = true;
			hideArmsShotgun = true;
			hideArmsCrowBar = true;
		}
		else if(targetHandSlot == 1 && flashlight)
		{
			// Show flashlight
			hideArmsFlashlight = false;
			hideArmsShotgun = true;
			hideArmsCrowBar = true;
		}
		else if(targetHandSlot == 2  && shotgun)
		{
			// Show Shotgun
			hideArmsFlashlight = true;
			hideArmsShotgun = false;
			hideArmsCrowBar = true;
			showBulletCountUI = true;
			showCrossHairUI = true;
		}	
		else if (targetHandSlot == 3 && crowBar)
		{
			// Show CrowBar
			hideArmsFlashlight = true;
			hideArmsShotgun = true;
			hideArmsCrowBar = false;
			showCrossHairUI = true;
		}
		else
		{
			return;
		}
		
		gameMode->AmmoUI(showBulletCountUI, shotgunBulletCount);
		gameMode->ReticleUI(showCrossHairUI);
		canSwitch = false;


		if (flashlight && flashlight->IsFlashlightOn())
		{
			FlashlightToggle();
		}
	

		currentHandSlot = targetHandSlot;
	}
}

int AMainCharacter::GetTargetHandSlot(float val)
{
	int32 targetHandSlot = currentHandSlot;
	if (val < 0.f)
	{
		targetHandSlot = currentHandSlot - 1;
		WrapNumberWithinRange(targetHandSlot, 0, items.Num() - 1);
		while (targetHandSlot != currentHandSlot && !items[targetHandSlot])
		{
			targetHandSlot--;
			WrapNumberWithinRange(targetHandSlot, 0, items.Num() - 1);
		}
	}
	else
	{
		targetHandSlot = currentHandSlot + 1;
		WrapNumberWithinRange(targetHandSlot, 0, items.Num() - 1);
		while (targetHandSlot != currentHandSlot && !items[targetHandSlot])
		{
			targetHandSlot++;
			WrapNumberWithinRange(targetHandSlot, 0, items.Num() - 1);
		}
	}
	return targetHandSlot;
}

void AMainCharacter::WrapNumberWithinRange(int32& num, int32 min, int32 max)
{
	if (num < min)
	{
		num = max;
	}
	else if (num > max)
	{
		num = min;
	}
}

void AMainCharacter::AimIn()
{
	isAiming = true;

	if (gameMode)
		gameMode->ReticleUI(false);

	
}

void AMainCharacter::AimOut()
{
	isAiming = false;

	if (gameMode)
		gameMode->ReticleUI(true);
}


void AMainCharacter::ResolveMovement()
{
	if (isCrouchingKeyDown)
	{
		SetMovement(EMovement::Crouching);
	}
	else
	{
		SetMovement(EMovement::Standing);
	}
}

bool AMainCharacter::AreStaminaEventsPlaying()
{
	return !FAkAudioDevice::Get()->IsEventIDActive(startSprintEvent) && !FAkAudioDevice::Get()->IsEventIDActive(heavySprintEvent) && !FAkAudioDevice::Get()->IsEventIDActive(gentleSprintEvent) && !FAkAudioDevice::Get()->IsEventIDActive(depletedSprintEvent);

}

void AMainCharacter::FlashlightToggle()
{
	if(flashlight && currentHandSlot == 1)
	{
		flashlight->Toggle();
	}
}

void AMainCharacter::TogglePause()
{
	if(isAiming)
		AimOut();

	if (gameMode)
	{
		gameMode->TogglePause(this);
	}


}

#pragma endregion 

void AMainCharacter::PickedUpKey()
{
	numberOfKeys++;
	numberOfKeys = FMath::Min(numberOfKeys, 3);
	FAkAudioDevice::Get()->SetRTPCValue(*FString("Num_of_Keys"), numberOfKeys, 300, this);
	
}

void AMainCharacter::StopPlayerSounds()
{
	FAkAudioDevice::Get()->StopPlayingID(startSprintEvent, 300);
	FAkAudioDevice::Get()->StopPlayingID(gentleSprintEvent, 300);
	FAkAudioDevice::Get()->StopPlayingID(heavySprintEvent, 300);
	FAkAudioDevice::Get()->StopPlayingID(depletedSprintEvent, 300);
}
