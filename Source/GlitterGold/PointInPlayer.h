// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointInPlayer.generated.h"

UCLASS()
class GLITTERGOLD_API APointInPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APointInPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
