// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractionWidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	cameraComponent->SetupAttachment(GetMesh());
	cameraComponent->bUsePawnControlRotation = true;


}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = crouchSpeed;
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformCheck();

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
	Crouch();
}

void AMainCharacter::CrouchReleased()
{
	UnCrouch();
}

#pragma endregion 


