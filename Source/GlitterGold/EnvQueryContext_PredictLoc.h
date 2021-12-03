// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvQueryContext_PredictLoc.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UEnvQueryContext_PredictLoc : public UEnvQueryContext
{
	GENERATED_BODY()

protected:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
	
};
