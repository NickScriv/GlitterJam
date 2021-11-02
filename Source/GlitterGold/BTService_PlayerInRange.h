// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_PlayerInRange.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UBTService_PlayerInRange : public UBTService
{
	GENERATED_BODY()
public:

		UBTService_PlayerInRange();

		UPROPERTY(EditAnywhere)
		float killDistance = 100.f;

		UPROPERTY(EditAnywhere)
			FBlackboardKeySelector  bbKey_InRange;

protected:
	void OnBecomeRelevant
	(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
