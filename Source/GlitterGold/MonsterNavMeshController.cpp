// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterNavMeshController.h"
#include "Monster.h"
#include "MainCharacter.h"
#include "Components/ArrowComponent.h"

// TODO: Delete this class
AMonsterNavMeshController::AMonsterNavMeshController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AMonsterNavMeshController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMonsterNavMeshController::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
}

void AMonsterNavMeshController::OnOverlapEnd(AActor* overlappedActor, AActor* otherActor)
{
}



