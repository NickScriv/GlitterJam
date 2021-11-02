// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_StartMonsterBehavior.h"
#include "Monster.h"
#include "MonsterAIController.h"

void UAnimNotify_StartMonsterBehavior::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//UE_LOG(LogTemp, Warning, TEXT("Noify"));
	Super::Notify(MeshComp, Animation);
	
	AMonster* monster = Cast<AMonster>(MeshComp->GetOwner());

	if(!monster)
	{
		//UE_LOG(LogTemp, Warning, TEXT("No Monster"));
		return;
	}


	if(!monster)
	{
		//UE_LOG(LogTemp, Error, TEXT("Monster not found from mesh component!!"));
		return;
	}

	AMonsterAIController* controller = Cast<AMonsterAIController>(monster->GetController());

	if (!controller)
	{
		//UE_LOG(LogTemp, Error, TEXT("controller not found from monster!!"));
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("controller start"));
	controller->StartMonsterBehavior();
}