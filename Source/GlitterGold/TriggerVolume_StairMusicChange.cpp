// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerVolume_StairMusicChange.h"

#include "AkAudioDevice.h"
#include "MainCharacter.h"

ATriggerVolume_StairMusicChange::ATriggerVolume_StairMusicChange()
{

}

void ATriggerVolume_StairMusicChange::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddUniqueDynamic(this, &ATriggerVolume_StairMusicChange::OnOverlapBegin);
}

void ATriggerVolume_StairMusicChange::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		AMainCharacter* player = Cast<AMainCharacter>(otherActor);

		if (player)
		{
			FAkAudioDevice::Get()->SetRTPCValue(*FString("Downstairs_Upstairs_Stairwell"), rtpcValue, 200, player);
			this->SetActorEnableCollision(false);
			otherTrigger->SetActorEnableCollision(true);
			//UE_LOG(LogTemp, Warning, TEXT("Change level music with value: %i"), rtpcValue);
		}
	}
}
