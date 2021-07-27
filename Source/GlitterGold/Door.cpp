// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "AkAudioEvent.h"
#include <AkAudio/Public/AkAudioDevice.h>

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ADoor::PlayOpenCloseSound(UAkAudioEvent* in_pEvent)
{
	FAkAudioDevice::Get()->PostEventAtLocation(in_pEvent, GetActorLocation(), GetActorRotation(), GetWorld());
	//FAkAudioDevice::Get()->PostEvent(text, this);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}



