// Fill out your copyright notice in the Description page of Project Settings.


#include "PointInPlayer.h"

// Sets default values
APointInPlayer::APointInPlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APointInPlayer::BeginPlay()
{
	Super::BeginPlay();

	bGenerateOverlapEventsDuringLevelStreaming = 1;
	
}

