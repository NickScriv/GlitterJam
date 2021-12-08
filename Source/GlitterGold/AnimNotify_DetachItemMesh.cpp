// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_DetachItemMesh.h"
#include "MainCharacter.h"

void UAnimNotify_DetachItemMesh::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	//UE_LOG(LogTemp, Warning, TEXT("Detach Mesh!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));

	AMainCharacter* player = Cast<AMainCharacter>(MeshComp->GetOwner());

	if (!player)
	{
		//UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_DetachItemMesh: No Player found"));
		return;
	}

	/*if (meshToDetach == 1)
	{
		player->HideFlashlight();
		//UE_LOG(LogTemp, Warning, TEXT("Hide Flashlight"));
	}
	else if (meshToDetach == 2)
	{
		player->HideShotgun();
		//UE_LOG(LogTemp, Warning, TEXT("Hide Shotgun"));
	}
	else if (meshToDetach == 3)
	{
		player->HideCrowBar();
		//UE_LOG(LogTemp, Warning, TEXT("Hide CrowBar"));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Wrong mesh value for detach mesh"));
	}*/

	player->HideCrowBar();
	player->HideFlashlight();
	player->HideShotgun();


	if (player->currentHandSlot == 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("next item is nothing, set canSwitch to true"));
		player->canSwitch = true;
	}
		
}