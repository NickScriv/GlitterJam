// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerVolume_StairMusicChange.h"

#include "AkAudioDevice.h"
#include "DrawDebugHelpers.h"
#include "MainCharacter.h"

ATriggerVolume_StairMusicChange::ATriggerVolume_StairMusicChange()
{

}

void ATriggerVolume_StairMusicChange::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ATriggerVolume_StairMusicChange::OnOverlapBegin);

	DrawDebugBox(GetWorld(), GetActorLocation(), Brush->Bounds.BoxExtent, FColor::Orange, true, -1, 0, 5);
}

void ATriggerVolume_StairMusicChange::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		AMainCharacter* player = Cast<AMainCharacter>(otherActor);

		if (player)
		{
			//FAkAudioDevice::Get()->SetRTPCValue(*FString("Downstairs_Upstairs_Stairwell"), 2, 300, this);
			FAkAudioDevice::Get()->SetRTPCValue(*FString("Downstairs_Upstairs_Stairwell"), rtpcValue, 300, player);
			this->SetActorEnableCollision(false);
			otherTrigger->SetActorEnableCollision(true);
			UE_LOG(LogTemp, Warning, TEXT("Change level music with value: %i"), rtpcValue);
		}
	}
}
