// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_PredictingStatus.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UBTTaskNode_PredictingStatus : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool status;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector  bbKey_Status;

protected:
	virtual EBTNodeResult::Type ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
