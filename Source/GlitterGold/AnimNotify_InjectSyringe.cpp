// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_InjectSyringe.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"

void UAnimNotify_InjectSyringe::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp->GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_InjectSyringe: No parent found"));
		return;
	}

	FAkAudioDevice::Get()->PostEvent("Inject_Cure", MeshComp->GetOwner());
}