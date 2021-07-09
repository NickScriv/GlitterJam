// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MonsterFootstep.h"
//#include "../Plugins/Wwise/Source/AkAudio/Classes/AkGameplayStatics.h"
#include "Monster.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"

void UAnimNotify_MonsterFootstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AMonster* monster = Cast<AMonster>(MeshComp->GetOwner());

	FAkAudioDevice::Get()->PostEvent("Play_Enemy_Footstep", monster);
}
