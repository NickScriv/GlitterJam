// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ToggleMonsterPhysics.h"
#include "GlitterGameModeBase.h"
#include "Monster.h"

void UAnimNotify_ToggleMonsterPhysics::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AMonster* monster = Cast<AMonster>(MeshComp->GetOwner());

	if (!monster)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_ToggleMonsterPhysics: No monster found"));
		return;
	}

	if (physicsOn)
	{
		monster->GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(FName("pelvis"), monster->blendPhysics, false, false);

		AGlitterGameModeBase* gameMode = Cast<AGlitterGameModeBase>(UGameplayStatics::GetGameMode(this));

		if (!gameMode)
		{
			UE_LOG(LogTemp, Error, TEXT("UBTTask_RandomLocation: No game mode found!"));
			return;
		}

		gameMode->monsterInCaution = 0.f;
	}
	else
	{
		monster->GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(FName("pelvis"), blend, false, false);
	}

}