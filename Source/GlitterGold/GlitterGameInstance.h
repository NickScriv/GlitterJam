// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GlitterGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UGlitterGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void Init() override;

public:

	UPROPERTY(BlueprintReadWrite)
	bool monsterKilled = false;

	UPROPERTY(BlueprintReadWrite)
	bool gameEnding = false;
};
