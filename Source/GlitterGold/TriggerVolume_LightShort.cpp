// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerVolume_LightShort.h"
#include "MainCharacter.h"
#include "CeilingLight.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkGameplayStatics.h"

void ATriggerVolume_LightShort::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ATriggerVolume_LightShort::OnOverlapBegin);
}


void ATriggerVolume_LightShort::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && otherActor != this && !playerEntered)
	{
		AMainCharacter* player = Cast<AMainCharacter>(otherActor);
		if (player)
		{
			playerEntered = true;
			for (ACeilingLight* light : lights)
			{
				if (light)
				{
					light->StartLightFlicker();
				}
			}
			//turnOffLights.BindUFunction(this, TEXT("TurnOffLights"));
			//UAkGameplayStatics::PostEvent(in_pEventLightsOff, player, AkCallbackType::AK_EndOfEvent, turnOffLights);
			GetWorldTimerManager().SetTimer(lightShortTimer, this, &ATriggerVolume_LightShort::TurnOffLights, lightFlickerTime, false);
		}
	}
}

void ATriggerVolume_LightShort::TurnOffLights()
{
	UE_LOG(LogTemp, Warning, TEXT("Turn off lights"));

	for (ACeilingLight* light : lights)
	{
		if (light)
		{
			light->ShutOffLight();
		}
	}
	Destroy();
}
