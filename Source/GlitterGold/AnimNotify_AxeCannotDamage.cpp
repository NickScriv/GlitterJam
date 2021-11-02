// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AxeCannotDamage.h"
#include "MainCharacter.h"

void UAnimNotify_AxeCannotDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AMainCharacter* player = Cast<AMainCharacter>(MeshComp->GetOwner());

	if (!player)
	{
		//UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_AxeCannotDamage: No Player found"));
		return;
	}

	player->axeCanDamage = false;
}