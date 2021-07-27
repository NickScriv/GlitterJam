// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class GLITTERGOLD_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	void PlayOpenCloseSound(class UAkAudioEvent* in_pEvent);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:



};
