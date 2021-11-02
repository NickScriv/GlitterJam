// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryContext_Player.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UEnvQueryContext_Player::ProvideContext(FEnvQueryInstance & QueryInstance, FEnvQueryContextData & ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	AMainCharacter* player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (player)
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, player);
	}
}