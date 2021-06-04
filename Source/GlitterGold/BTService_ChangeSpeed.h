// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UBTService_ChangeSpeed : public UBTService
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere)
	float speed;

	UBTService_ChangeSpeed();


protected:
	void OnBecomeRelevant
	(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) override;
	
};
