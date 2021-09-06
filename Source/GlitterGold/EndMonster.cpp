// Fill out your copyright notice in the Description page of Project Settings.


#include "EndMonster.h"

// Sets default values
AEndMonster::AEndMonster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	root = CreateDefaultSubobject<USceneComponent>("Root");
	root->SetupAttachment(RootComponent);

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	mesh->SetupAttachment(root);

}

USkeletalMeshComponent* AEndMonster::GetMesh()
{
	return mesh;
}

// Called when the game starts or when spawned
void AEndMonster::BeginPlay()
{
	Super::BeginPlay();

	SetActorHiddenInGame(true);
	
}

