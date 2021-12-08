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
#include "GlitterGameModeBase.h"
#include "LockPick.h"
#include "NavModifierVolume.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/StaticMeshActor.h"

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

	UAkComponent* comp = FAkAudioDevice::Get()->GetAkComponent(this->GetOwner()->GetRootComponent(), FName(), NULL, EAttachLocation::KeepRelativeOffset);

	if (comp)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Door ak comp"));
		comp->OcclusionCollisionChannel = ECollisionChannel::ECC_GameTraceChannel7;
		comp->OcclusionRefreshInterval = 0.0f;
	}

	gameMode = Cast<AGlitterGameModeBase>(UGameplayStatics::GetGameMode(this));

	if ((interaction = Cast<UInteractionWidgetComponent>(GetOwner()->GetComponentByClass(UInteractionWidgetComponent::StaticClass()))) == nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("DoorOpenClose: Interaction is null for %s"), *GetOwner()->GetName());
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
		if(box->GetName() == "NavModBox")
		{
			navMeshBox = box;
		}
	}

	if (interaction)
	{	
		interaction->OnInteract.AddUniqueDynamic(this, &UDoorOpenClose::InteractDoor);
		interaction->OnBeginFocus.AddUniqueDynamic(this, &UDoorOpenClose::BeginFocusDoor);
		interaction->OnEndFocus.AddUniqueDynamic(this, &UDoorOpenClose::EndFocusDoor);
		interaction->OnEndInteract.AddUniqueDynamic(this, &UDoorOpenClose::EndInteractDoor);
		interaction->OnBeginInteract.AddUniqueDynamic(this, &UDoorOpenClose::BeginInteractDoor);
	}

	if(keyToDoor)
	{
		keyToDoor->OnPickedUpKey.AddUniqueDynamic(this, &UDoorOpenClose::PlayerPickedUpKey);
	}

	if (lockPick)
	{
		lockPick->onPickedUpLockPick.AddUniqueDynamic(this, &UDoorOpenClose::PlayerPickedUpLockPick);
		lockPick->onLockPickUsed.AddUniqueDynamic(this, &UDoorOpenClose::LockPickUsed);
	}
	
	if(!navLinkProxyEnter)
	{
		//UE_LOG(LogTemp, Error, TEXT("DoorOpenClose: Nav link proxy enter for door is null for %s"), *GetOwner()->GetName());
		return;
	}

	if (!navLinkProxyExit)
	{
		//UE_LOG(LogTemp, Error, TEXT("DoorOpenClose: Nav link proxy exit for door is null for %s"), *GetOwner()->GetName());
		return;
	}

	navLinkProxyEnter->OnSmartLinkReached.AddUniqueDynamic(this, &UDoorOpenClose::MonsterReachedNavLink);
	navLinkProxyExit->OnSmartLinkReached.AddUniqueDynamic(this, &UDoorOpenClose::MonsterReachedNavLink);

	if (locked)
	{
		navLinkProxyEnter->SetSmartLinkEnabled(false);
		navLinkProxyExit->SetSmartLinkEnabled(false);
	}
	else
	{
		navLinkProxyEnter->SetSmartLinkEnabled(true);
		navLinkProxyExit->SetSmartLinkEnabled(true);
	}

	initialYaw = this->GetRelativeRotation().Yaw;
	currentYaw = initialYaw;
	openAngle += currentYaw;
	
}


// Called every frame
void UDoorOpenClose::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (gameMode && gameMode->playerKilled)
	{
		SetComponentTickEnabled(false);
		ADoor* parent = Cast<ADoor>(this->GetOwner());

		if(parent)
			parent->StopDoorSounds();

		return;
	}

	if (openTime < 1)
	{
		openTime += DeltaTime * doorSpeed;

		if(openTime > 0.9 && checkClosed)
		{
			/*if (monsterController)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Resume move request: %i"), currentMove.GetID());
				monsterController->ResumeMove(currentMove);
			}*/

			if (monster)
			{
				monster->GetCharacterMovement()->Activate();
			}
				
			//UE_LOG(LogTemp, Warning, TEXT("Resume Move"));
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
		{
			//UE_LOG(LogTemp, Warning, TEXT("Close protal"));
			portal->ClosePortal();
		}
			
		//GetWorld()->GetTimerManager().SetTimer(timerHandleNavLinks, this, &UDoorOpenClose::EnableNavLinks, 0.75f, false);
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
			//UE_LOG(LogTemp, Warning, TEXT("Door open: %i, name: %s"), id, *this->GetOwner()->GetName());
		}*/

		if(portal)
		{
			//UE_LOG(LogTemp, Warning, TEXT("open protal"));
			portal->OpenPortal();
		}
		navLinkProxyEnter->SetSmartLinkEnabled(false);
		navLinkProxyExit->SetSmartLinkEnabled(false);

		//GetWorld()->GetTimerManager().SetTimer(timerHandleNavLinks, this, &UDoorOpenClose::DisableNavLinks, 0.75f, false);
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
		FAkAudioDevice::Get()->PostEvent("Locked_Door", character);
		return;	
	}
	
	
	if(playerHasKey && locked)
	{
		FAkAudioDevice::Get()->PostEvent("Stop_Door_Unlocking", character);
		UnlockDoor();
	}
	else if (playerHasLockPick && locked)
	{
		FAkAudioDevice::Get()->PostEvent("Stop_Lock_Picking", character);
		FAkAudioDevice::Get()->PostEvent("Lock_Pick_Break", character);
		lockPick->onLockPickUsed.Broadcast();
		UnlockDoor();
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
		FAkAudioDevice::Get()->PostEvent("Play_Door_Unlocking", character);
	}
	else if (locked && playerHasLockPick)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Play lock pick sound"));
		FAkAudioDevice::Get()->PostEvent("Play_Lock_Picking", character);
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
		FAkAudioDevice::Get()->PostEvent("Stop_Door_Unlocking", character);
	}
	else if (locked && playerHasLockPick)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Stop lock pick sound"));
		FAkAudioDevice::Get()->PostEvent("Stop_Lock_Picking", character);
	}
}


void UDoorOpenClose::MonsterReachedNavLink(AActor* MovingActor, const FVector& DestinationPoint)
{
	if(!monster)
		monster = Cast<AMonster>(MovingActor);

	if(monster)
	{
		navLinkProxyEnter->ResumePathFollowing(monster);
		navLinkProxyExit->ResumePathFollowing(monster);
		
		if(!locked && openTime >= 1)
		{
			if(!monsterController)
				monsterController = Cast<AMonsterAIController>(monster->GetController());

			//currentMove = monsterController->GetCurrentMoveRequestID();
			//UE_LOG(LogTemp, Warning, TEXT("Pause move request: %i"), currentMove.GetID());
			//monsterController->PauseMove(currentMove);
			
			monster->GetCharacterMovement()->Deactivate();
			
			if (!open)
				checkClosed = true;
			
			OpenDoor();
		}
			
	}
}

void UDoorOpenClose::UnlockNavMesh()
{
	/*navLinkProxyEnter->SetSmartLinkEnabled(true);
	navLinkProxyExit->SetSmartLinkEnabled(true);*/
}

void UDoorOpenClose::ChangeMonsterPath()
{

	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMonster::StaticClass());

	if (actor)
	{
		if (AMonster* monst = Cast<AMonster>(actor))
		{
			const int32 index = FMath::RandRange(0, possibleMonsterPaths.Num() - 1);
			//UE_LOG(LogTemp, Warning, TEXT("CHANGE PATHSHSH"));
			monst->ChangeCurrentPath(possibleMonsterPaths[index].pathPoints);
		}
	}
}

void UDoorOpenClose::UnlockDoor()
{
	locked = false;
	
	interaction->interactionTime = 0.f;

	if (actorToDestroyWhenUnlocked)
	{
		actorToDestroyWhenUnlocked->Destroy();
	}

	navMeshBox->AreaClass = doorNavAreaClass;

	//GetWorld()->GetTimerManager().SetTimer(timerHandleChangeMonsterPath, this, &UDoorOpenClose::ChangeMonsterPath, 5.0f, false);
	ChangeMonsterPath();
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

	
	//UE_LOG(LogTemp, Error, TEXT("Lock pick for door"));
	
}

void UDoorOpenClose::LockPickUsed()
{
	playerHasLockPick = false;

	if (!playerHasKey)
		interaction->interactionTime = 0.f;
}

