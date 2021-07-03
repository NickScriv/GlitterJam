// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerVolume_StairMusicChange.h"

#include "AkAudioDevice.h"
#include "DrawDebugHelpers.h"
#include "MainCharacter.h"

ATriggerVolume_StairMusicChange::ATriggerVolume_StairMusicChange()
{
	OnActorBeginOverlap.AddDynamic(this, &ATriggerVolume_StairMusicChange::OnOverlapBegin);
}

void ATriggerVolume_StairMusicChange::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugBox(GetWorld(), GetActorLocation(), Brush->Bounds.BoxExtent, FColor::Orange, true, -1, 0, 5);
}

void ATriggerVolume_StairMusicChange::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		AMainCharacter* player = Cast<AMainCharacter>(otherActor);

		if (player)
		{
			FAkAudioDevice::Get()->SetRTPCValue(*FString("Downstair_Upstairs_Stairwell"), rtpcValue, 300, this);
			this->SetActorEnableCollision(false);
			otherTrigger->SetActorEnableCollision(true);
			UE_LOG(LogTemp, Warning, TEXT("Change level music with value: %i"), rtpcValue);
		}
	}
}
