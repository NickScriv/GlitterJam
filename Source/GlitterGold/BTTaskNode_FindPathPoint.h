// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_FindPathPoint.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UBTTaskNode_FindPathPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector  bbKey_TargetLoc;

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector  bbKey_PathPointWaitTime;	

protected:
	virtual EBTNodeResult::Type ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
