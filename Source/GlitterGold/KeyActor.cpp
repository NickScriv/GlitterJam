// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyActor.h"
#include "InteractionWidgetComponent.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"

// Sets default values
AKeyActor::AKeyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	keyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Key Mesh");
	keyMesh->SetupAttachment(RootComponent);

	/*interaction = CreateDefaultSubobject<UInteractionWidgetComponent>("Interaction Component");
	interaction->SetupAttachment(RootComponent);*/

}

// Called when the game starts or when spawned
void AKeyActor::BeginPlay()
{
	Super::BeginPlay();
	if ((interaction = Cast<UInteractionWidgetComponent>(GetComponentByClass(UInteractionWidgetComponent::StaticClass()))) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Interaction is null!!!"));
		return;
	}

	interaction->OnInteract.AddDynamic(this, &AKeyActor::PickedUp);
	
}

void AKeyActor::PickedUp(class AMainCharacter* character)
{
	OnPickedUpKey.Broadcast();
	FAkAudioDevice::Get()->PostEvent("Key_Acquired", this);
	Destroy();
}

