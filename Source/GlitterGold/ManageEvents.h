// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ManageEvents.generated.h"

UCLASS()
class GLITTERGOLD_API AManageEvents : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AManageEvents();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:

	UPROPERTY(EditAnywhere)
		TArray<class AMonster*> firstScareEventMonsters;

	UPROPERTY(EditAnywhere)
		TArray<class ATriggerVolume*> firstScareEventTriggers;

};
