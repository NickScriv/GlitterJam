// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryContext_CachedPlayerLoc.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterAIController.h"
#include "Monster.h"

void UEnvQueryContext_CachedPlayerLoc::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMonster::StaticClass());
	bool succ = false;

	if (actor)
	{
		if (AMonster* monst = Cast<AMonster>(actor))
		{

			AMonsterAIController* controller = Cast<AMonsterAIController>(monst->GetController());

			if (controller && controller->predictNavHelper)
			{
				succ = true;
				UEnvQueryItemType_Actor::SetContextHelper(ContextData, controller->playerPredictNavHelper);
			}

		}

	}

	if (!succ)
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, actor);
	}


}