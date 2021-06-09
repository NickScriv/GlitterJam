// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ChangeMusic.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UBTService_ChangeMusic : public UBTService
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FString musicChange;

	UBTService_ChangeMusic();


protected:
	void OnBecomeRelevant
	(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) override;
	
};
