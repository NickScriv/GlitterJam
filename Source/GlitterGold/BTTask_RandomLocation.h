// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UBTTask_RandomLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float radius;

	UPROPERTY(EditAnywhere)
		float walkSpeed;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector  bbKey;



protected:
		virtual EBTNodeResult::Type ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
