// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_DoneShooting.h"
#include "MainCharacter.h"

void UAnimNotify_DoneShooting::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AMainCharacter* player = Cast<AMainCharacter>(MeshComp->GetOwner());

	if (!player)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AnimNotifyArmsHideGet: No Player found"));
		return;
	}

	/*player->isShooting = false;
	//UE_LOG(LogTemp, Warning, TEXT("Done Shooting"));*/

}