// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_MoveToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UBTTaskNode_MoveToPlayer : public UBTTaskNode
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
