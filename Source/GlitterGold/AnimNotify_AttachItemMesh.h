// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AttachItemMesh.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UAnimNotify_AttachItemMesh : public UAnimNotify
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		int32 meshToAttach = 0;

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
