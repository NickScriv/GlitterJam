// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorOpenClose.h"
#include "InteractionWidgetComponent.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkGameplayStatics.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"

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
		UE_LOG(LogTemp, Warning, TEXT("Interaction is null!!!"));
	}
	
	

	if (interaction)
	{
		
		interaction->OnInteract.AddDynamic(this, &UDoorOpenClose::InteractDoor);
		interaction->OnBeginFocus.AddDynamic(this, &UDoorOpenClose::FocusDoor);
	}

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
	}
	FRotator newRot = FRotator(0.0, FMath::Lerp(currentYaw, initialYaw + (open ? openAngle : 0.0f), openTime), 0.0);

	this->SetRelativeRotation(newRot);
}

void UDoorOpenClose::OpenDoor()
{
	if (open)
	{
		FAkAudioDevice::Get()->PostEvent("Door_Close", this->GetOwner());
	}
	else
	{
		
		FAkAudioDevice::Get()->PostEvent("Door_Open", this->GetOwner());
	}
	open = !open;
	FocusDoor(nullptr);
	
	currentYaw = this->GetRelativeRotation().Yaw;
	openTime = 0;
}


void UDoorOpenClose::InteractDoor(class AMainCharacter* character)
{
	if (openTime >= 1)
		OpenDoor();
}

void UDoorOpenClose::FocusDoor(class AMainCharacter* character)
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

