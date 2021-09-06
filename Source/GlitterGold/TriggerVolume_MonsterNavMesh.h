// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "TriggerVolume_MonsterNavMesh.generated.h"

class ANavModifierVolume;
class UNavArea;
/**
 * 
 */
UCLASS()
class GLITTERGOLD_API ATriggerVolume_MonsterNavMesh : public ATriggerVolume
{
	GENERATED_BODY()

public:
	ATriggerVolume_MonsterNavMesh();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnOverlapNavBegin(AActor* overlappedActor, AActor* otherActor);

	UFUNCTION()
		void OnOverlapNavEnd(AActor* overlappedActor, AActor* otherActor);

	UPROPERTY()
		class UArrowComponent* arrow;

	UPROPERTY(EditAnywhere, Category = "Mesh Volumes")
		TArray<ANavModifierVolume*> downStairMeshVolumes;

	UPROPERTY(EditAnywhere, Category = "Mesh Volumes")
		TArray<ANavModifierVolume*> upStairMeshVolumes;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UNavArea> defaultNavAreaClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UNavArea> nullNavAreaClass;
	
};
