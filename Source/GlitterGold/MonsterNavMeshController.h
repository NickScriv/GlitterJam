// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterNavMeshController.generated.h"

UCLASS()
class GLITTERGOLD_API AMonsterNavMeshController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterNavMeshController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UFUNCTION()
		void OnOverlapBegin(class AActor* overlappedActor, class AActor* otherActor);

	UFUNCTION()
		void OnOverlapEnd(class AActor* overlappedActor, class AActor* otherActor);

	UPROPERTY()
		class UArrowComponent* arrow;


};
