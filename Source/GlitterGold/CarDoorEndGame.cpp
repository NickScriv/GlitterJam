// Fill out your copyright notice in the Description page of Project Settings.


#include "CarDoorEndGame.h"
#include "Components/BoxComponent.h"
#include "InteractionWidgetComponent.h"
#include "GlitterGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"
#include "GlitterGameModeBase.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"

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

	gameMode = Cast<AGlitterGameModeBase>(UGameplayStatics::GetGameMode(this));
}

void ACarDoorEndGame::EndGame(class AMainCharacter* character)
{
	UE_LOG(LogTemp, Warning, TEXT("ENDING!!!!!!!!!!!!!!!!!!"));

	UGlitterGameInstance* gameInstance = Cast<UGlitterGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (gameInstance)
	{
		if (gameInstance->monsterKilled)
		{
			character->DisablePlayer();

			if (gameMode)
			{
				gameMode->FadeOutHUD();
			}

			GetWorldTimerManager().SetTimer(goodEndCutsceneHandle, this, &ACarDoorEndGame::TriggerGoodEnding, 2.f, false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Monster not Killed ending"));
		}
	}
}

void ACarDoorEndGame::ToMainMenu()
{
	gameMode->ScreenToBlack();
	UGameplayStatics::OpenLevel(this, FName("MainMenu"));
	UE_LOG(LogTemp, Error, TEXT("Switch to main menu"));
}

void ACarDoorEndGame::TriggerGoodEnding()
{
	// Setup the sequence player

	//Sequence Play
	UE_LOG(LogTemp, Error, TEXT("Above sequencer"));

	if (SequenceGoodEnding && SequencePlayer == nullptr)
		SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), SequenceGoodEnding, FMovieSceneSequencePlaybackSettings(), SequenceActor);

	if (SequenceActor)
	{
		SequenceActor->PlaybackSettings.bPauseAtEnd = true;
		SequenceActor->PlaybackSettings.bHideHud = true;
	}

	if (SequencePlayer)
	{
		SequencePlayer->OnStop.AddDynamic(this, &ACarDoorEndGame::ToMainMenu);
		SequencePlayer->Play();
		UE_LOG(LogTemp, Error, TEXT("Play sequencer"));
	}

	if (gameMode)
	{
		gameMode->FadeInHUD();
	}
}


