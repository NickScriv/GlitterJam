// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AxeCanDamage.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UAnimNotify_AxeCanDamage : public UAnimNotify
{
	GENERATED_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere)
		float axeRange;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* normalHit;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* monsterHit;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* woodHit;
	
};
