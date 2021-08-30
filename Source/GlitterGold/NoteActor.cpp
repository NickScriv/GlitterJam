// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteActor.h"
#include "AkAudioDevice.h"
#include "GlitterGameModeBase.h"
#include "InteractionWidgetComponent.h"
#include "NotesUserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANoteActor::ANoteActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	noteMesh = CreateDefaultSubobject<UStaticMeshComponent>("Note Mesh");
	SetRootComponent(noteMesh);
}

// Called when the game starts or when spawned
void ANoteActor::BeginPlay()
{
	Super::BeginPlay();

	if ((interaction = Cast<UInteractionWidgetComponent>(GetComponentByClass(UInteractionWidgetComponent::StaticClass()))) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Interaction is null!!!"));
		return;
	}

	interaction->OnInteract.AddDynamic(this, &ANoteActor::PickedUpNote);

	if(!noteWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Note widget is null!!!"));
		return;
	}

}

void ANoteActor::PickedUpNote(class AMainCharacter* character)
{

	if (!noteWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Note widget is null!!!"));
		return;
	}

	AGlitterGameModeBase* gameMode = Cast<AGlitterGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (gameMode)
	{
		gameMode->isReadingNote = true;
	}
	
	//Play open note sound
	FAkAudioDevice::Get()->PostEvent("Pick_Up_Note", this);
	canExit = false;
	GetWorldTimerManager().SetTimer(exitNoteTimer, this, &ANoteActor::CanExitNoteExpire, 1.f, false);
	BindToInput();
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	noteWidget->AddToViewport();
}

void ANoteActor::BindToInput()
{
	InputComponent = NewObject<UInputComponent>(this);
	InputComponent->RegisterComponent();
	if (InputComponent)
	{
		InputComponent->BindAction("Interact", IE_Pressed, this, &ANoteActor::CloseNote).bExecuteWhenPaused = true;
		EnableInput(GetWorld()->GetFirstPlayerController());
	}
}

void ANoteActor::CloseNote()
{
	if(!canExit)
		return;

	AGlitterGameModeBase* gameMode = Cast<AGlitterGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (gameMode)
	{
		gameMode->isReadingNote = false;
	}
	
	FAkAudioDevice::Get()->PostEvent("Put_Down_Note", this);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	noteWidget->RemoveFromParent();
	DisableInput(GetWorld()->GetFirstPlayerController());
}

void ANoteActor::CanExitNoteExpire()
{
	canExit = true;
	timeElapsed = 0.f;
}

void ANoteActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(timeElapsed <= delayTimer && !canExit)
	{
		timeElapsed += DeltaTime;
	}

	if(timeElapsed > delayTimer)
	{
		CanExitNoteExpire();
	}

}


