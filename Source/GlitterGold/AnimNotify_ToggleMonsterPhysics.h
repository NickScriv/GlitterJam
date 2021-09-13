// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ToggleMonsterPhysics.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UAnimNotify_ToggleMonsterPhysics : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool physicsOn;

	UPROPERTY(EditAnywhere)
	float blend;

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
