// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttachItemMesh.h"
#include "MainCharacter.h"

void UAnimNotify_AttachItemMesh::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AMainCharacter* player = Cast<AMainCharacter>(MeshComp->GetOwner());

	if (!player)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimNotifyArmsHideGet: No Player found"));
		return;
	}

	if (meshToAttach == 1)
	{
		player->ShowFlashlight();
		UE_LOG(LogTemp, Warning, TEXT("Show Flashlight"));
	}
	else if (meshToAttach == 2)
	{
		player->ShowShotgun();
		UE_LOG(LogTemp, Warning, TEXT("Show Shotgun"));
	}
	else if (meshToAttach == 3)
	{
		player->ShowCrowBar();
		UE_LOG(LogTemp, Warning, TEXT("Show CrowBar"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Wrong mesh value for attach mesh"));
	}

}