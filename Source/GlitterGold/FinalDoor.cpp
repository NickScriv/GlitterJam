// Fill out your copyright notice in the Description page of Project Settings.


#include "FinalDoor.h"
#include "InteractionWidgetComponent.h"
#include "MainCollectible.h"
#include "Kismet/GameplayStatics.h"
#include "GlitterGameModeBase.h"
#include "MainCharacter.h"
#include "GlitterGameInstance.h"
#include "Monster.h"

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
		//UE_LOG(LogTemp, Error, TEXT("Interaction is null!!!"));
		return;
	}
	
	interaction->SetWidgetClass(nullptr);
	
}

void AFinalDoor::TriggerEnd(AMainCharacter* character)
{
	character->DisablePlayer();

	AGlitterGameModeBase* gameMode = Cast<AGlitterGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (gameMode)
	{
		gameMode->FadeOutHUD();
	}

	UGlitterGameInstance* gameInstance = Cast<UGlitterGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (gameInstance)
	{
		gameInstance->gameEnding = true;
	}
	
	SwitchToEnding();

	TArray<AActor*> monsters;
	UGameplayStatics::GetAllActorsOfClass(this, AMonster::StaticClass(), monsters);

	if (monsters.Num() != 1)
	{
		//UE_LOG(LogTemp, Error, TEXT("There only should be 1 monster in the level"));
		return;
	}

	for (AActor* actor : monsters)
	{
		AMonster* monster = Cast<AMonster>(actor);

		if (monster)
			monster->DetachFromControllerPendingDestroy();
	}
}

void AFinalDoor::InitInteraction()
{
	interaction->SetWidgetClass(interarctionWidgetClass);
	interaction->OnInteract.AddUniqueDynamic(this, &AFinalDoor::TriggerEnd);
}

