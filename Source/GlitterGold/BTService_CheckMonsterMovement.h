// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckMonsterMovement.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UBTService_CheckMonsterMovement : public UBTService
{
	GENERATED_BODY()

	UBTService_CheckMonsterMovement();

protected:
	void OnBecomeRelevant
	(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) override;
	
};
