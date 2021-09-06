// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_BadEnding.h"
#include "GlitterGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotify_BadEnding::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp->GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_BadEnding: No mesh comp owner found"));
		return;
	}

	AGlitterGameModeBase* gameMode = Cast<AGlitterGameModeBase>(UGameplayStatics::GetGameMode(MeshComp->GetOwner()));

	if (gameMode)
	{
		gameMode->BadEnding();
	}
}