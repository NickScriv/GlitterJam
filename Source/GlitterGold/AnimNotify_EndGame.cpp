// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_EndGame.h"
#include "Kismet/GameplayStatics.h"
#include "GlitterGameModeBase.h"
#include "Engine/World.h"

void UAnimNotify_EndGame::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	// TODO: play event here
	AGlitterGameModeBase* gameMode = Cast<AGlitterGameModeBase>(UGameplayStatics::GetGameMode(MeshComp->GetWorld()));

	if(gameMode)
		gameMode->EndGame();


}

