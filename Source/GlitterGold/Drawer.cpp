// Fill out your copyright notice in the Description page of Project Settings.


#include "Drawer.h"

#include "AkAudioDevice.h"
#include "InteractionWidgetComponent.h"
#include "MainCharacter.h"

// Sets default values
ADrawer::ADrawer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADrawer::BeginPlay()
{
	Super::BeginPlay();

	if ((interaction = Cast<UInteractionWidgetComponent>(GetComponentByClass(UInteractionWidgetComponent::StaticClass()))) == nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("Drawer: Interaction is null for %s"), *GetName());
		return;
	}

	interaction->OnInteract.AddDynamic(this, &ADrawer::InteractDrawer);
	interaction->OnBeginFocus.AddDynamic(this, &ADrawer::BeginFocusDrawer);

	startLoc = GetActorLocation();;
	endLoc = startLoc;
	endLoc.X -= distanceToOpen;

	if (openCloseCurveFloat)
	{
		FOnTimelineFloat timelineProgress;
		timelineProgress.BindUFunction(this, FName("TimelineProgressOpenClose"));
		openCloseTimeline.AddInterpFloat(openCloseCurveFloat, timelineProgress);
		FOnTimelineEvent finished = FOnTimelineEvent();
		finished.BindUFunction(this, FName("FinishOpenClose"));
		openCloseTimeline.SetTimelineFinishedFunc(finished);
		openCloseTimeline.SetLooping(false);
	}
}


bool ADrawer::IsOpening()
{
	return opening;
}

// Called every frame
void ADrawer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	openCloseTimeline.TickTimeline(DeltaTime);

}

void ADrawer::InteractDrawer(class AMainCharacter* character)
{
	if(!opening)
		OpenCloseDrawer();
}

void ADrawer::BeginFocusDrawer(AMainCharacter* character)
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

void ADrawer::TimelineProgressOpenClose(float val)
{
	//FHitResult hit = FHitResult();
	SetActorRelativeLocation(FMath::Lerp(startLoc, endLoc, val));
	
	
	/*if(AMainCharacter* player = Cast<AMainCharacter>(hit.Actor))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Trace start: %f, %f, %f"), hit.TraceStart.X, hit.TraceStart.Y, hit.TraceStart.Z);
		FVector unitDirection = (hit.TraceEnd - hit.TraceStart).GetSafeNormal();
		float dist = FVector::Distance(hit.TraceEnd, hit.TraceStart) - hit.Distance;
		unitDirection *= dist;
		player->AddActorWorldOffset(unitDirection);
	}*/
		
	
}

void ADrawer::FinishOpenClose()
{
	opening = false;
}

void ADrawer::OpenCloseDrawer()
{
	open = !open;
	opening = true;
	if (open)
	{
		FAkAudioDevice::Get()->PostEvent("Drawers_Open", this);
		openCloseTimeline.PlayFromStart();
	}
	else
	{
		FAkAudioDevice::Get()->PostEvent("Drawers_Close", this);
		openCloseTimeline.Reverse();
	}

	BeginFocusDrawer(nullptr);
}

