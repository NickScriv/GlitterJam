// Fill out your copyright notice in the Description page of Project Settings.


#include "CarDoorEndGame.h"
#include "Components/BoxComponent.h"
#include "InteractionWidgetComponent.h"
#include "GlitterGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"

// Sets default values
ACarDoorEndGame::ACarDoorEndGame()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	box = CreateDefaultSubobject<UBoxComponent>("Box");
	box->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ACarDoorEndGame::BeginPlay()
{
	Super::BeginPlay();

	if ((interaction = Cast<UInteractionWidgetComponent>(GetComponentByClass(UInteractionWidgetComponent::StaticClass()))) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Interaction is null!!!"));
		return;
	}

	interaction->OnInteract.AddDynamic(this, &ACarDoorEndGame::EndGame);
}

void ACarDoorEndGame::EndGame(class AMainCharacter* character)
{
	UE_LOG(LogTemp, Warning, TEXT("End Game"));
	UGlitterGameInstance* gameInstance = Cast<UGlitterGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (gameInstance)
	{
		if (gameInstance->monsterKilled)
		{
			UE_LOG(LogTemp, Warning, TEXT("Monster Killed ending"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Monster not Killed ending"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Instance not found"));
	}

	Destroy();
}

