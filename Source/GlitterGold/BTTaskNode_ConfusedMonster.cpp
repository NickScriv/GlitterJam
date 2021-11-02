// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ConfusedMonster.h"
#include "GlitterGameModeBase.h"
/*#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Monster.h"*/

EBTNodeResult::Type UBTTaskNode_ConfusedMonster::ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	/*if (!OwnerComp.GetAIOwner())
		return EBTNodeResult::Failed;

	AMonster* monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!monster)
		return EBTNodeResult::Failed;*/

	AGlitterGameModeBase* gameMode = Cast<AGlitterGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (!gameMode)
	{
		//UE_LOG(LogTemp, Error, TEXT("UBTTask_RandomLocation: No game mode found!"));
		return EBTNodeResult::Failed;
	}

	gameMode->monsterInCaution = 2.f;
	return EBTNodeResult::Succeeded;
}
