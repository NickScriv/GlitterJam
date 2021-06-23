// Fill out your copyright notice in the Description page of Project Settings.


#include "AkSpatialAudioVolume_Reverb.h"
#include "GlitterGameModeBase.h"
#include "Monster.h"
#include "MainCharacter.h"
#include "PointInMonster.h"
#include "PointInPlayer.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"
#include "Kismet/GameplayStatics.h"

class APointInPlayer;

AAkSpatialAudioVolume_Reverb::AAkSpatialAudioVolume_Reverb()
{
	// listen to event
	OnActorBeginOverlap.AddDynamic(this, &AAkSpatialAudioVolume_Reverb::OnOverlapBegin);

}

void AAkSpatialAudioVolume_Reverb::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		if (APointInMonster* pointInMonster = Cast<APointInMonster>(otherActor))
		{
			// Monster stepped in
			UE_LOG(LogTemp, Warning, TEXT("PointInMonster detected!"));
			FAkAudioDevice::Get()->SetRTPCValue(*FString("Enemy_Footsteps_Surface_Type"), footStepSurface, 0, pointInMonster->GetParentActor());

		}
		else if (APointInPlayer* pointInPlayer = Cast<APointInPlayer>(otherActor))
		{
			//UE_LOG(LogTemp, Warning, TEXT("PointInPlayer detected!"));
			// Player stepped in
			AMainCharacter* player = Cast<AMainCharacter>(pointInPlayer->GetParentActor());

			if (!player)
			{
				UE_LOG(LogTemp, Warning, TEXT("Reverb Spatial Volume: Player actor not found"));
				return;
			}

			AGlitterGameModeBase* gameMode = Cast<AGlitterGameModeBase>(UGameplayStatics::GetGameMode(this));
			gameMode->queuedMusic = musicName;
			UE_LOG(LogTemp, Warning, TEXT("Change footstep surface: %i"), footStepSurface);
			FAkAudioDevice::Get()->SetRTPCValue(*FString("Footsteps_Surface_Type"), footStepSurface, 0, player);


			if (FMath::IsNearlyEqual(gameMode->monsterInCaution, 0.0f, 0.2f))
			{
				FAkAudioDevice::Get()->PostEvent(gameMode->queuedMusic, player);
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Player nor monster entered the reverb trigger %s"), *otherActor->GetName());
		}
	}
	
}

