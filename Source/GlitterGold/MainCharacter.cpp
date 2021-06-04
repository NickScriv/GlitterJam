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
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleColl = GetCapsuleComponent();
	cameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	cameraComponent->SetupAttachment(capsuleColl);
	cameraComponent->bUsePawnControlRotation = true;

	//GetMesh()->SetupAttachment(cameraComponent);
	
	//GetMesh()->AttachToComponent(cameraComponent, FAttachmentTransformRules::KeepWorldTransform);

	

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->AttachToComponent(cameraComponent, FAttachmentTransformRules::KeepWorldTransform);
	standCameraHeight = cameraComponent->GetRelativeLocation().Z;
	crouchCameraHeight = standCameraHeight * crouchScale;

	standCapsuleHeight = capsuleColl->GetScaledCapsuleHalfHeight();
	crouchCapsuleHeight = capsuleColl->GetScaledCapsuleHalfHeight() * crouchScale;

	if (crouchCurveFloat)
	{
		FOnTimelineFloat timelineProgress;
		timelineProgress.BindUFunction(this, FName("TimelineProgressCrouch"));
		crouchCurveTimeline.AddInterpFloat(crouchCurveFloat, timelineProgress);
		crouchCurveTimeline.SetLooping(false);
	}

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformCheck();

	
	crouchCurveTimeline.TickTimeline(DeltaTime);

	if (died)
	{
		FRotator res = FMath::RInterpTo(cameraComponent->GetComponentRotation(), rotateDeath, DeltaTime, deathRotTime);

		cameraComponent->SetWorldRotation(res);
	}

	if (movement == EMovement::Standing && isSprintingKeyDown && GetVelocity().Size() > 0.5f && !GetCharacterMovement()->IsFalling())
	{
		stamina -= staminaDepletion * DeltaTime;
		stamina = FMath::Clamp(stamina, 0.f, 100.f);
	}
	else
	{
		stamina += staminaRegen * DeltaTime;
		stamina = FMath::Clamp(stamina, 0.f, 100.f);
	}

	if (stamina < 1.f && movement == EMovement::Standing)
	{
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		GetWorldTimerManager().SetTimer(timerFootstep, this, &AMainCharacter::PlayFootStep, footStepTimeWalk, true);

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

	//PlayerInputComponent->BindAction("TestNot", IE_Pressed, this, &AMainCharacter::NotTest);

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
	//PerformCheck();
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
	FVector camLoc = cameraComponent->GetRelativeLocation();
	camLoc.Z = FMath::Lerp(standCameraHeight, crouchCameraHeight, val);
	cameraComponent->SetRelativeLocation(camLoc);

	float height = FMath::Lerp(standCapsuleHeight, crouchCapsuleHeight, val);
	capsuleColl->SetCapsuleHalfHeight(height);
}

void AMainCharacter::NotTest()
{
	AGlitterGameModeBase* gameMode = Cast<AGlitterGameModeBase>(UGameplayStatics::GetGameMode(this));

	if(gameMode)
		gameMode->AddNotification(FText::FromString("Notification!"));
}

void AMainCharacter::Died(AMonster* monster)
{
	if ((movement == EMovement::Crouching))
	{
		monsterOffsetLookAt = 0;
	}
	
	SetMovement(EMovement::Standing);
	DisableInput(UGameplayStatics::GetPlayerController(this, 0));
	FVector monsterLookAt = monster->GetActorLocation();
	monsterLookAt = FVector(monsterLookAt.X, monsterLookAt.Y , monsterLookAt.Z + monsterOffsetLookAt);
	rotateDeath = UKismetMathLibrary::FindLookAtRotation(cameraComponent->GetComponentLocation(), monsterLookAt);
	cameraComponent->bUsePawnControlRotation = false;
	GetCharacterMovement()->DisableMovement();
	died = true;
	
}

void AMainCharacter::PlayFootStep()
{
	UGameplayStatics::PlaySoundAtLocation(this, footSound, GetActorLocation());

	if (EMovement::Crouching == movement)
	{
		UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), crouchLoudness, this, 0, FName("Noise"));
	}
	else if(IsSprinting())
	{
		UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), sprintLoudness, this, 0, FName("Noise"));
	}
	else
	{
		UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), walkLoudness, this, 0, FName("Noise"));
	}
}

void AMainCharacter::ProcessFootStep()
{
	FVector vel = GetVelocity();

	if ((vel.X > 2.f || vel.X < -2.f || vel.Y > 2.f || vel.Y < -2.f))
	{
		if (playFootStep)
		{
			float footTime;

			if (movement == EMovement::Crouching)
			{
				footTime = footStepTimeCrouch;
			}
			else if (IsSprinting())
			{
				footTime = footStepTimeSprint;
			}
			else
			{
				footTime = footStepTimeWalk;
			}
			GetWorldTimerManager().SetTimer(timerFootstep, this, &AMainCharacter::PlayFootStep, footTime, true);
			playFootStep = false;
		}

	}
	else
	{
		GetWorldTimerManager().ClearTimer(timerFootstep);
		playFootStep = true;
	}
}

bool AMainCharacter::IsSprinting()
{
	return stamina > 1 && isSprintingKeyDown;
}

void AMainCharacter::MoveForward(float val)
{
	AddMovementInput(GetActorForwardVector(), val);
	ProcessFootStep();
}

void AMainCharacter::MoveRight(float val)
{
	AddMovementInput(GetActorRightVector(), val);
	ProcessFootStep();
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

	if (movement != EMovement::Standing || GetCharacterMovement()->IsFalling())
		return;


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
	isSprintingKeyDown = true;

	if (movement != EMovement::Standing || GetCharacterMovement()->IsFalling())
		return;

	GetCharacterMovement()->MaxWalkSpeed = sprintSpeed;
	GetWorldTimerManager().SetTimer(timerFootstep, this, &AMainCharacter::PlayFootStep, footStepTimeSprint, true);
}

void AMainCharacter::SprintReleased()
{
	isSprintingKeyDown = false;

	if (movement != EMovement::Standing || GetCharacterMovement()->IsFalling())
		return;

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	GetWorldTimerManager().SetTimer(timerFootstep, this, &AMainCharacter::PlayFootStep, footStepTimeWalk, true);
}

void AMainCharacter::BeginCrouch()
{
	// crouch

	crouchCurveTimeline.Play();
	GetCharacterMovement()->MaxWalkSpeed = crouchSpeed;
	GetWorldTimerManager().SetTimer(timerFootstep, this, &AMainCharacter::PlayFootStep, footStepTimeCrouch, true);
	
}

void AMainCharacter::EndCrouch()
{
	// stand
	crouchCurveTimeline.Reverse();
	


	if (isSprintingKeyDown && stamina > 10.f)
	{
		GetCharacterMovement()->MaxWalkSpeed = sprintSpeed;
		GetWorldTimerManager().SetTimer(timerFootstep, this, &AMainCharacter::PlayFootStep, footStepTimeSprint, true);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		GetWorldTimerManager().SetTimer(timerFootstep, this, &AMainCharacter::PlayFootStep, footStepTimeWalk, true);
	}
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

#pragma endregion 


