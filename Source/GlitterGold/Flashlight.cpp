// Fill out your copyright notice in the Description page of Project Settings.


#include "Flashlight.h"
#include "Components/SpotLightComponent.h"
#include "InteractionWidgetComponent.h"
#include "MainCharacter.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"

// Sets default values
AFlashlight::AFlashlight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Flashlight Mesh");
	mesh->SetupAttachment(RootComponent);
	
	spotLight = CreateDefaultSubobject<USpotLightComponent>("Light");
	spotLight->SetupAttachment(mesh);

}

void AFlashlight::Toggle()
{	
	flashlightOn = !flashlightOn;

	if(flashlightOn)
	{
		FAkAudioDevice::Get()->PostEvent("Flashlight_On", this);
	}
	else
	{
		FAkAudioDevice::Get()->PostEvent("Flashlight_Off", this);
	}
	
	spotLight->SetVisibility(flashlightOn);
}


bool AFlashlight::IsFlashlightOn()
{
	return flashlightOn;
}

// Called when the game starts or when spawned
void AFlashlight::BeginPlay()
{
	Super::BeginPlay();

	spotLight->SetVisibility(flashlightOn);

	if ((interaction = Cast<UInteractionWidgetComponent>(GetComponentByClass(UInteractionWidgetComponent::StaticClass()))) == nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("Interaction is null!!!"));
		return;
	}

	interaction->OnInteract.AddDynamic(this, &AFlashlight::PickedUp);
	
}

void AFlashlight::PickedUp(AMainCharacter* character)
{
	character->SpawnFlashlight();
	FAkAudioDevice::Get()->PostEvent("Pickup_Flashlight", this);
	Destroy();
}

