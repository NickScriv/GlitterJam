// Fill out your copyright notice in the Description page of Project Settings.


#include "WallLight.h"
#include "Components/PointLightComponent.h"

// Sets default values
AWallLight::AWallLight()
{
 	// TODO: set to false if dont need for flickering lights
	PrimaryActorTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Light Mesh");
	mesh->SetupAttachment(RootComponent);

	light = CreateDefaultSubobject<UPointLightComponent>("Light");
	light->SetupAttachment(mesh);
}

// Called when the game starts or when spawned
void AWallLight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWallLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

