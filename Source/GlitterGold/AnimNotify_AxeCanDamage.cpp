// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AxeCanDamage.h"
#include "MainCharacter.h"

void UAnimNotify_AxeCanDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AMainCharacter* player = Cast<AMainCharacter>(MeshComp->GetOwner());

	if (!player)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_AxeCanDamage: No Player found"));
		return;
	}

	player->axeCanDamage = true;
}
