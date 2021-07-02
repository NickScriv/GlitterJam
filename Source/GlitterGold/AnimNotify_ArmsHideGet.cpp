// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ArmsHideGet.h"
#include "MainCharacter.h"

void UAnimNotify_ArmsHideGet::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UE_LOG(LogTemp, Warning, TEXT("Noify"));
	Super::Notify(MeshComp, Animation);

	AMainCharacter* player = Cast<AMainCharacter>(MeshComp->GetOwner());

	if (!player)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimNotifyArmsHideGet: No Player found"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("AnimNotifyArmsHideGet: Excecute"));
	player->canSwitch = true;
}