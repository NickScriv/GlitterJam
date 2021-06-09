// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MonsterFootstep.h"
/*#include "../Plugins/Wwise/Source/AkAudio/Classes/AkGameplayStatics.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"*/

void UAnimNotify_MonsterFootstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	//FAkAudioDevice::Get()->PostEvent("Play_Enemy_Footstep", MeshComp->GetOwner());
}
