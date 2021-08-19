// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_DisableMonsterColl.h"
#include "Monster.h"
#include "Components/CapsuleComponent.h"

void UAnimNotify_DisableMonsterColl::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AMonster* monster = Cast<AMonster>(MeshComp->GetOwner());

	if(monster && monster->GetCapsuleComponent())
		monster->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
}