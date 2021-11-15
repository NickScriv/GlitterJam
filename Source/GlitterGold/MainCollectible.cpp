// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCollectible.h"
#include "InteractionWidgetComponent.h"
#include "AkAudioDevice.h"
#include "FinalDoor.h"

// Sets default values
AMainCollectible::AMainCollectible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainCollectible::BeginPlay()
{
	Super::BeginPlay();

	if ((interaction = Cast<UInteractionWidgetComponent>(GetComponentByClass(UInteractionWidgetComponent::StaticClass()))) == nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("Interaction is null!!!"));
		return;
	}

	interaction->OnInteract.AddUniqueDynamic(this, &AMainCollectible::PickedUp);
	
}

// Called every frame
void AMainCollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainCollectible::PickedUp(class AMainCharacter* character)
{
	// Trigger final door
	//UE_LOG(LogTemp, Error, TEXT("wow gregretgtregt4rgmpile"));
	FAkAudioDevice::Get()->PostEvent("Pickup_Flashlight", this);
	finalDoor->InitInteraction();
	character->InjectSyringe();
	Destroy();
}

