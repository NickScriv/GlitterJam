// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathPoint.generated.h"

UCLASS()
class GLITTERGOLD_API APathPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathPoint();

	UPROPERTY(EditAnywhere)
	float waitTime;

	UPROPERTY(EditAnywhere)
	float waitTimeDeviation;

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* sphere;

	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* arrow;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
