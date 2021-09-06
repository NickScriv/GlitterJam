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
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void StartGameInstance() override;

public:

	UGlitterGameInstance(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite)
	bool monsterKilled = false;

	UPROPERTY(BlueprintReadWrite)
	bool gameEnding = false;
};
