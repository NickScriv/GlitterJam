// Fill out your copyright notice in the Description page of Project Settings.


#include "FinalDoor.h"

#include "InteractionWidgetComponent.h"
#include "MainCollectible.h"

// Sets default values
AFinalDoor::AFinalDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AFinalDoor::BeginPlay()
{
	Super::BeginPlay();

	if ((interaction = Cast<UInteractionWidgetComponent>(GetComponentByClass(UInteractionWidgetComponent::StaticClass()))) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Interaction is null!!!"));
		return;
	}
	
	interaction->SetWidgetClass(nullptr);
	
}

void AFinalDoor::TriggerEnd(class AMainCharacter* character)
{
	// Trigger end Cutscene
	UE_LOG(LogTemp, Warning, TEXT("END GAME!!!"));
}

void AFinalDoor::InitInteraction()
{
	interaction->SetWidgetClass(interarctionWidgetClass);
	interaction->OnInteract.AddDynamic(this, &AFinalDoor::TriggerEnd);
}

