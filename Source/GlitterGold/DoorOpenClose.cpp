// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorOpenClose.h"

#include "AIController.h"
#include "AITypes.h"
#include "AkAcousticPortal.h"
#include "MainCharacter.h"
#include "InteractionWidgetComponent.h"
#include "Monster.h"
#include "KeyActor.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "NavModifierComponent.h"
#include "Navigation/NavLinkProxy.h"
#include "NavMesh/NavMeshPath.h"
#include "Door.h"
#include "LockPick.h"



// Sets default values for this component's properties
UDoorOpenClose::UDoorOpenClose()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UDoorOpenClose::BeginPlay()
{
	Super::BeginPlay();

	if ((interaction = Cast<UInteractionWidgetComponent>(GetOwner()->GetComponentByClass(UInteractionWidgetComponent::StaticClass()))) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("DoorOpenClose: Interaction is null for %s"), *GetOwner()->GetName());
	}

	if ((portal = Cast<UAkPortalComponent>(GetOwner()->GetComponentByClass(UAkPortalComponent::StaticClass()))) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("DoorOpenClose: Portal is null!!!"));
	}
	
	if(portal)
	{
		portal->ClosePortal();
	}

	TArray<UActorComponent*> boxes;
	GetOwner()->GetComponents(UBoxComponent::StaticClass(), boxes);
	
	for(UActorComponent* item : boxes)
	{
		UBoxComponent* box = Cast<UBoxComponent>(item);
		if(box->GetName() == "BoxPortal")
		{
			if (UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass())))
			{
				box->SetWorldLocationAndRotation(mesh->GetComponentLocation(), mesh->GetComponentRotation());
			}
		}
	}

	if (interaction)
	{	
		interaction->OnInteract.AddDynamic(this, &UDoorOpenClose::InteractDoor);
		interaction->OnBeginFocus.AddDynamic(this, &UDoorOpenClose::BeginFocusDoor);
		interaction->OnEndFocus.AddDynamic(this, &UDoorOpenClose::EndFocusDoor);
		interaction->OnEndInteract.AddDynamic(this, &UDoorOpenClose::EndInteractDoor);
		interaction->OnBeginInteract.AddDynamic(this, &UDoorOpenClose::BeginInteractDoor);
	}

	if(keyToDoor)
	{
		keyToDoor->OnPickedUpKey.AddDynamic(this, &UDoorOpenClose::PlayerPickedUpKey);
	}

	if (lockPick)
	{
		lockPick->onPickedUpLockPick.AddDynamic(this, &UDoorOpenClose::PlayerPickedUpLockPick);
		lockPick->onLockPickUsed.AddDynamic(this, &UDoorOpenClose::LockPickUsed);
	}
	
	if(!navLinkProxyEnter)
	{
		UE_LOG(LogTemp, Error, TEXT("DoorOpenClose: Nav link proxy enter for door is null for %s"), *GetOwner()->GetName());
		return;
	}

	if (!navLinkProxyExit)
	{
		UE_LOG(LogTemp, Error, TEXT("DoorOpenClose: Nav link proxy exit for door is null for %s"), *GetOwner()->GetName());
		return;
	}

	/*navLinkProxyEnter->SetSmartLinkEnabled(true);
	navLinkProxyExit->SetSmartLinkEnabled(true);*/
	navLinkProxyEnter->OnSmartLinkReached.AddDynamic(this, &UDoorOpenClose::MonsterReachedNavLink);
	navLinkProxyExit->OnSmartLinkReached.AddDynamic(this, &UDoorOpenClose::MonsterReachedNavLink);

	initialYaw = this->GetRelativeRotation().Yaw;
	currentYaw = initialYaw;
	openAngle += currentYaw;
	
}


// Called every frame
void UDoorOpenClose::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (openTime < 1)
	{
		openTime += DeltaTime * doorSpeed;

		if(openTime > 0.9 && checkClosed)
		{
			if(monsterController)
				monsterController->ResumeMove(FAIRequestID::CurrentRequest);
			UE_LOG(LogTemp, Warning, TEXT("Resume Move"));
			checkClosed = false;
		}
	}
	FRotator newRot = FRotator(0.0, FMath::Lerp(currentYaw, initialYaw + (open ? openAngle : 0.0f), openTime), 0.0);

	this->SetRelativeRotation(newRot);
	
}

void UDoorOpenClose::OpenDoor()
{
	//int32 id;
	ADoor* parent = Cast<ADoor>(this->GetOwner());
	if (open)
	{
		if (parent)
		{
			parent->PlayOpenCloseSound(in_pEventClose);
		}
		
		if (portal)
			portal->ClosePortal();

		navLinkProxyEnter->SetSmartLinkEnabled(true);
		navLinkProxyExit->SetSmartLinkEnabled(true);
	}
	else
	{

		if (parent)
		{
			parent->PlayOpenCloseSound(in_pEventOpen);
		}
		
		/*id = FAkAudioDevice::Get()->PostEvent("Door_Open", this->GetOwner());
		if (this->GetOwner())
		{
			UE_LOG(LogTemp, Warning, TEXT("Door open: %i, name: %s"), id, *this->GetOwner()->GetName());
		}*/

		if(portal)
			portal->OpenPortal();
		navLinkProxyEnter->SetSmartLinkEnabled(false);
		navLinkProxyExit->SetSmartLinkEnabled(false);
	}
	open = !open;
	BeginFocusDoor(nullptr);
	
	currentYaw = this->GetRelativeRotation().Yaw;
	openTime = 0;
}


void UDoorOpenClose::InteractDoor(class AMainCharacter* character)
{
	if(locked && !playerHasKey && !playerHasLockPick)
	{
		// Play locked door sound
		FAkAudioDevice::Get()->PostEvent("Locked_Door", this->GetOwner());
		return;	
	}
	else if(playerHasKey && locked)
	{
		FAkAudioDevice::Get()->PostEvent("Stop_Door_Unlocking", this->GetOwner());
		locked = false;
		interaction->interactionTime = 0.f;
	}
	else if (playerHasLockPick && locked)
	{
		FAkAudioDevice::Get()->PostEvent("Stop_Door_Unlocking", this->GetOwner());
		locked = false;
		interaction->interactionTime = 0.f;
		// TODO: Break lock pick
		lockPick->onLockPickUsed.Broadcast();
	}
	
	if (openTime >= 1)
		OpenDoor();
}

void UDoorOpenClose::BeginFocusDoor(class AMainCharacter* character)
{
	if(!locked)
	{
		if (open)
		{
			interaction->SetInteractableActionText(FText::FromString("Close"));
		}
		else
		{
			interaction->SetInteractableActionText(FText::FromString("Open"));
		}
	}
	else if(playerHasKey)
	{
		interaction->SetInteractableActionText(FText::FromString("Unlock"));
		interaction->SetInteractableKeyAction(FText::FromString(""));
	}
	else if (playerHasLockPick)
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

void UDoorOpenClose::BeginInteractDoor(AMainCharacter* character)
{
	// Start playing unlocking sound
	if (playerHasKey && locked)
	{
		FAkAudioDevice::Get()->PostEvent("Play_Door_Unlocking", this->GetOwner());
	}
}

// May not need to do anything here
void UDoorOpenClose::EndFocusDoor(AMainCharacter* character)
{
	
}

void UDoorOpenClose::EndInteractDoor(AMainCharacter* character)
{
	// Stop playing unlocking sound
	if(playerHasKey && locked)
	{
		FAkAudioDevice::Get()->PostEvent("Stop_Door_Unlocking", this->GetOwner());
	}
}


void UDoorOpenClose::MonsterReachedNavLink(AActor* MovingActor, const FVector& DestinationPoint)
{
	UE_LOG(LogTemp, Warning, TEXT("Something enetered Nav link!!!!!!!!!!!!!"));
	AMonster* monster = Cast<AMonster>(MovingActor);
	if(monster)
	{
		navLinkProxyEnter->ResumePathFollowing(monster);
		navLinkProxyExit->ResumePathFollowing(monster);
		
		if(!locked && openTime >= 1)
		{
			monsterController = Cast<AAIController>(monster->GetController());
			monsterController->PauseMove(FAIRequestID::CurrentRequest);
			
			if (!open)
				checkClosed = true;
			
			OpenDoor();
		}
			
	}
}

void UDoorOpenClose::PlayerPickedUpKey()
{
	if (locked)
	{
		playerHasKey = true;
		interaction->interactionTime = unlockDoorTime;
	}
}

void UDoorOpenClose::PlayerPickedUpLockPick()
{
	if (!locked || playerHasKey)
		return;

	playerHasLockPick = true;
	interaction->interactionTime = unlockLockPickTime;

	
	UE_LOG(LogTemp, Error, TEXT("Lock pick for door"));
	
}

void UDoorOpenClose::LockPickUsed()
{
	playerHasLockPick = false;

	if (!playerHasKey)
		interaction->interactionTime = 0.f;
}

