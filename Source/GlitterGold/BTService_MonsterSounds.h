// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_MonsterSounds.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UBTService_MonsterSounds : public UBTService
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FString monsterSound;

		UBTService_MonsterSounds();


protected:
	void OnBecomeRelevant
	(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) override;
	
};
