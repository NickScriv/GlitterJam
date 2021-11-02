// Fill out your copyright notice in the Description page of Project Settings.


#include "LockPick.h"
#include "InteractionWidgetComponent.h"
#include "MainCharacter.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"
#include "GlitterGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "DoorOpenClose.h"

// Sets default values
ALockPick::ALockPick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	lockPickMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	lockPickMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ALockPick::BeginPlay()
{
	Super::BeginPlay();

	if ((interaction = Cast<UInteractionWidgetComponent>(GetComponentByClass(UInteractionWidgetComponent::StaticClass()))) == nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("Interaction is null!!!"));
		return;
	}

	interaction->OnInteract.AddDynamic(this, &ALockPick::PickedUp);
	
}

void ALockPick::PickedUp(AMainCharacter* character)
{
	FAkAudioDevice::Get()->PostEvent("Pickup_Flashlight", this);
	onPickedUpLockPick.Broadcast();
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	
}


