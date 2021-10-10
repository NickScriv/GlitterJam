#pragma once

#include "PathPoint.h"
#include "GlitterStructs.generated.h"

USTRUCT()
struct FPathPointArray
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		TArray<APathPoint*> pathPoints;
};