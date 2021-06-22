// Fill out your copyright notice in the Description page of Project Settings.


#include "PointInMonster.h"

// Sets default values
APointInMonster::APointInMonster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bGenerateOverlapEventsDuringLevelStreaming = 1;

}

// Called when the game starts or when spawned
void APointInMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

