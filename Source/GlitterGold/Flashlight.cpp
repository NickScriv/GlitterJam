// Fill out your copyright notice in the Description page of Project Settings.


#include "Flashlight.h"
#include "Components/SpotLightComponent.h"

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
	spotLight->SetVisibility(flashlightOn);
	// Play flashlight toggle sound
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
	
}

