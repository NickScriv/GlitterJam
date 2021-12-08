// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ToggleMonsterPhysics.h"
#include "GlitterGameModeBase.h"
#include "Monster.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotify_ToggleMonsterPhysics::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AMonster* monster = Cast<AMonster>(MeshComp->GetOwner());

	if (!monster)
	{
		//UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_ToggleMonsterPhysics: No monster found"));
		return;
	}

	monster->TogglePhysics(physicsOn);
}