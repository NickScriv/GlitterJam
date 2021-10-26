// Fill out your copyright notice in the Description page of Project Settings.


#include "Safe.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"
#include "InteractionWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Monster.h"
#include "LockPick.h"

// Sets default values
ASafe::ASafe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	SetRootComponent(mesh);
	//mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASafe::BeginPlay()
{
	Super::BeginPlay();
	if ((interaction = Cast<UInteractionWidgetComponent>(GetComponentByClass(UInteractionWidgetComponent::StaticClass()))) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Interaction is null in Safe!!!"));
		return;
	}

	interaction->OnInteract.AddDynamic(this, &ASafe::InteractSafe);
	interaction->OnBeginFocus.AddDynamic(this, &ASafe::BeginFocusSafe);
	interaction->OnEndInteract.AddDynamic(this, &ASafe::EndInteractSafe);
	interaction->OnBeginInteract.AddDynamic(this, &ASafe::BeginInteractSafe);

	interaction->interactionTime = 0.f;

	interaction->AttachToComponent(mesh, FAttachmentTransformRules::KeepWorldTransform, FName("Interaction"));

	if (lockPick)
	{
		lockPick->onPickedUpLockPick.AddDynamic(this, &ASafe::PickedUpLockPick);
		lockPick->onLockPickUsed.AddDynamic(this, &ASafe::LockPickUsed);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Lock Pick not found!!!"));
	}
	

}

void ASafe::InteractSafe(AMainCharacter* character)
{
	if (locked && !playerHasPick)
	{
		// Play locked door sound
		FAkAudioDevice::Get()->PostEvent("Safe_Locked", this);
		return;
	}
	else if (playerHasPick && locked)
	{
		FAkAudioDevice::Get()->PostEvent("Stop_Lock_Picking", this);
		locked = false;
		playerHasPick = false;
		FAkAudioDevice::Get()->PostEvent("Lock_Pick_Break", character);
		lockPick->onLockPickUsed.Broadcast();
		interaction->interactionTime = 0.f;
		interaction->OnInteract.RemoveAll(this);
		interaction->DestroyComponent();
		ChangeMonsterPath();
	}

	if (!open)
	{
		OpenSafe();
	}
		
}

void ASafe::BeginFocusSafe(AMainCharacter* character)
{
	if (!locked)
	{
		return;
	}
	else if (playerHasPick)
	{
		interaction->SetInteractableActionText(FText::FromString("Pick"));
		interaction->SetInteractableKeyAction(FText::FromString(""));
	}
	else
	{
		interaction->SetInteractableActionText(FText::FromString(""));
		interaction->SetInteractableKeyAction(FText::FromString("Locked"));
	}
}

void ASafe::BeginInteractSafe(AMainCharacter* character)
{
	// Start playing unlocking sound
	if (playerHasPick && locked)
	{
		FAkAudioDevice::Get()->PostEvent("Play_Lock_Picking", this);
	}
}

void ASafe::EndInteractSafe(AMainCharacter* character)
{
	// Stop playing unlocking sound
	if (playerHasPick && locked)
	{
		FAkAudioDevice::Get()->PostEvent("Stop_Lock_Picking", this);
	}
}

void ASafe::PickedUpLockPick()
{
	playerHasPick = true;
	interaction->interactionTime = unlockSafeTime;
}

void ASafe::LockPickUsed()
{
	playerHasPick = false;
	interaction->interactionTime = 0.f;
}

void ASafe::OpenSafe()
{
	open = true;
	// TODO: Play crank sound!!!
	//FAkAudioDevice::Get()->PostEventAtLocation(in_pEventOpen, GetActorLocation(), GetActorRotation(), GetWorld());
}

void ASafe::ChangeMonsterPath()
{
	if (possibleMonsterPaths.Num() < 1)
		return;

	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMonster::StaticClass());
	
	if (actor)
	{
		if (AMonster* monst = Cast<AMonster>(actor))
		{
			const int32 index = FMath::RandRange(0, possibleMonsterPaths.Num() - 1);

			monst->ChangeCurrentPath(possibleMonsterPaths[index].pathPoints);
		}
	}

}


