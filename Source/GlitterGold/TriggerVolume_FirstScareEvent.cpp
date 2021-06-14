// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerVolume_FirstScareEvent.h"
#include "DrawDebugHelpers.h"
#include "Engine/BrushShape.h"
#include "Monster.h"


ATriggerVolume_FirstScareEvent::ATriggerVolume_FirstScareEvent()
{
	// listen to event
	OnActorBeginOverlap.AddDynamic(this, &ATriggerVolume_FirstScareEvent::OnOverlapBegin);
}

void ATriggerVolume_FirstScareEvent::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugBox(GetWorld(), GetActorLocation(), Brush->Bounds.BoxExtent, FColor::Orange, true, -1, 0, 5);
}

void ATriggerVolume_FirstScareEvent::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
	if(otherActor && otherActor != this)
	{
		AMonster* monster = Cast<AMonster>(otherActor);

		if(!monster)
		{
			UE_LOG(LogTemp, Error, TEXT("Monster Not found in trigger"));
			return;
		}

		monster->startAnimation = true;

		Destroy();
	}
}
