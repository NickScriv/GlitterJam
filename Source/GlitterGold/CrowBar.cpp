// Fill out your copyright notice in the Description page of Project Settings.


#include "CrowBar.h"
#include "InteractionWidgetComponent.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"
#include "MainCharacter.h"
#include "Monster.h"
#include "Kismet/GameplayStatics.h"
#include "DestructibleActor.h"
#include "DestructibleComponent.h"
#include "Perception/AISense_Damage.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

// Sets default values
ACrowBar::ACrowBar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	crowBarMesh = CreateDefaultSubobject<USkeletalMeshComponent>("CrowBar Mesh");
	//crowBarMesh->SetupAttachment(RootComponent);
	SetRootComponent(crowBarMesh);

}

// Called when the game starts or when spawned
void ACrowBar::BeginPlay()
{
	Super::BeginPlay();
	
	if ((interaction = Cast<UInteractionWidgetComponent>(GetComponentByClass(UInteractionWidgetComponent::StaticClass()))) == nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("Interaction is null!!!"));
		return;
	}

	interaction->OnInteract.AddUniqueDynamic(this, &ACrowBar::PickedUp);
	
}

void ACrowBar::PickedUp(AMainCharacter* character)
{
	character->SpawnCrowBar();
	FAkAudioDevice::Get()->PostEvent("Pickup_Flashlight", this);
	Destroy();
}

