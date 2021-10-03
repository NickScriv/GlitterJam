// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CeilingLight.generated.h"

UCLASS()
class GLITTERGOLD_API ACeilingLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACeilingLight();

	UFUNCTION(BlueprintImplementableEvent)
	void StartLightFlicker();

	UFUNCTION(BlueprintImplementableEvent)
	void ShutOffLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
