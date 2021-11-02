// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ChangeBrakingAcc.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UBTService_ChangeBrakingAcc : public UBTService
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float brakeAcc;

	UBTService_ChangeBrakingAcc();


protected:
	void OnBecomeRelevant
	(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) override;
	
};
