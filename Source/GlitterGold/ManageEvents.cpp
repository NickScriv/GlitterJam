// Fill out your copyright notice in the Description page of Project Settings.


#include "ManageEvents.h"
#include "Engine/TriggerVolume.h"
#include "Monster.h"

// Sets default values
AManageEvents::AManageEvents()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AManageEvents::BeginPlay()
{
	Super::BeginPlay();

	if(firstScareEventMonsters.Num() != firstScareEventTriggers.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Trigger and monsters array must be equal for event manager to work!"));
		return;
	}

	if (firstScareEventMonsters.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("The event arrays are empty!!"));
		return;
	}

	const int32 index = FMath::RandRange(0, firstScareEventMonsters.Num() - 1);

	for(int32 i = 0; i < firstScareEventMonsters.Num(); i++)
	{
		
		if(i != index)
		{
			/*firstScareEventMonsters[i]->SetActorHiddenInGame(true);
			firstScareEventMonsters[i]->SetActorEnableCollision(false);
			firstScareEventMonsters[i]->SetActorTickEnabled(false);*/
			firstScareEventMonsters[i]->Destroy();
			
			/*firstScareEventTriggers[i]->SetActorHiddenInGame(true);
			firstScareEventTriggers[i]->SetActorEnableCollision(false);
			firstScareEventTriggers[i]->SetActorTickEnabled(false);*/
			firstScareEventTriggers[i]->Destroy();
		}
		else
		{
			firstScareEventMonsters[i]->SetActorHiddenInGame(false);
			firstScareEventMonsters[i]->SetActorEnableCollision(true);
			firstScareEventMonsters[i]->SetActorTickEnabled(true);

			firstScareEventTriggers[i]->SetActorHiddenInGame(false);
			firstScareEventTriggers[i]->SetActorEnableCollision(true);
			firstScareEventTriggers[i]->SetActorTickEnabled(true);
		}
	}
	
}


