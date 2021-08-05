// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/TriggerVolume.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"
#include "Components/CapsuleComponent.h"
#include "GlitterGameModeBase.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	if(firstEventScare)
		firstEventScare->OnActorBeginOverlap.AddDynamic(this, &AMonster::TriggerFirstEvent);
	else
		UE_LOG(LogTemp, Error, TEXT("AMonster: Trigger not found for scare event in monster!!"));

	passiveEvent = FAkAudioDevice::Get()->PostEvent("Play_Enemy_Passive_Sounds", this);

	mainPlayer = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if(!mainPlayer)
		UE_LOG(LogTemp, Error, TEXT("AMonster: Player not found!!!"));

	gameMode = Cast<AGlitterGameModeBase>(UGameplayStatics::GetGameMode(this));
}

void AMonster::TriggerFirstEvent(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		AMainCharacter* player = Cast<AMainCharacter>(otherActor);

		if (!player)
		{
			UE_LOG(LogTemp, Error, TEXT("AMonster: Player Not found in trigger"));
			return;
		}

		startAnimation = true;
	}
}

float AMonster::ScaleRange(float input, float inputLow, float inputHigh, float outputLow, float outputHigh)
{
	return ((input - inputLow) / (inputHigh - inputLow)) * (outputHigh - outputLow) + outputLow;
}

float AMonster::ReverseNumber(float num, float min, float max)
{
	return (max + min) - num;
}

void AMonster::KillMonster(FVector shotDir)
{
	if (gameMode)
		gameMode->monsterKilled = true;

	FAkAudioDevice::Get()->SetRTPCValue(*FString("Danger_Warning"), 0.f, 200, mainPlayer);

	if (AMainCharacter* player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		FAkAudioDevice::Get()->PostEvent("Play_Ambient_Music", player);
	}

	//GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	float dot = FVector::DotProduct(shotDir, GetActorForwardVector());

	if (dot >= 0.4f)
	{
		// Die Forwards
		deathStatus = EDeathStatus::DieForwards;

	}
	else if (dot <= -0.4f)
	{
		// Die Backwards
		deathStatus = EDeathStatus::DieBackwards;

	}
	else
	{
		// Die sideways

		float dotSide = FVector::DotProduct(shotDir, GetActorRightVector());

		if (dotSide < 0)
		{
			// Die left!?!?!
			deathStatus = EDeathStatus::DieLeft;
		}
		else
		{
			//Die right!?!?!?
			deathStatus = EDeathStatus::DieRight;
		}

	}

	DetachFromControllerPendingDestroy();
	// TODO: change to ragdoll??!?!
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AMonster::KillPlayer()
{
	AAIController* controller =  UAIBlueprintHelperLibrary::GetAIController(this);
	
	if (controller)
	{
		if(!mainPlayer)
		{
			mainPlayer = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		}
		
		if (mainPlayer)
		{
			mainPlayer->Died(this);
			controller->SetFocus(mainPlayer);
			FAkAudioDevice::Get()->PostEvent("Death_Music", mainPlayer);
			FAkAudioDevice::Get()->PostEvent("Death_Enemy_Sounds", this);
			
			mainPlayer->StopPlayerSounds();
			FVector playerLookAt = GetActorLocation();
			rotateKill= UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mainPlayer->GetActorLocation());

			killedPlayer = true;
		}
		controller->GetBrainComponent()->StopLogic("Dead");
	}

	GetCharacterMovement()->DisableMovement();

	if(gameMode)
		gameMode->playerKilled = true;
	

}

void AMonster::StopMonsterSounds()
{
	FAkAudioDevice::Get()->StopPlayingID(passiveEvent, 300);
	FAkAudioDevice::Get()->StopPlayingID(cautionEvent, 300);
	FAkAudioDevice::Get()->StopPlayingID(chaseEvent, 300);
}

void AMonster::PlayMosnterSoundEvent(FString event)
{
	if (event == "Play_Enemy_Passive_Sounds")
	{
		passiveEvent = FAkAudioDevice::Get()->PostEvent(*event, this);
	}
	else if (event == "Play_Enemy_Caution_Sounds")
	{
		cautionEvent = FAkAudioDevice::Get()->PostEvent(*event, this);
	}
	else
	{
		chaseEvent = FAkAudioDevice::Get()->PostEvent(*event, this);
	}
}

void AMonster::TakeDamage(const FVector& shotDir)
{
	health--;

	if(health <= 0.f)
		KillMonster(shotDir);
}

void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult hit;
	FCollisionQueryParams qParams;

	if (!gameMode->monsterKilled && GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), mainPlayer->GetCapsuleComponent()->GetComponentLocation(), ECC_Visibility, qParams))
	{
		if (Cast<AMainCharacter>(hit.Actor))
		{
			inLineOfPlayer = true;
			float dist = hit.Distance;
			dist = FMath::Clamp(dist, minDistanceAmbience, 700.f);
			dist = ReverseNumber(ScaleRange(dist, minDistanceAmbience, 700.f, 0.0f, 100.f), 0.0f, 100.f);
			FAkAudioDevice::Get()->SetRTPCValue(*FString("Danger_Warning"), dist, 200, mainPlayer);
		}
		else if(inLineOfPlayer)
		{
			inLineOfPlayer = false;
			FAkAudioDevice::Get()->SetRTPCValue(*FString("Danger_Warning"), 0, 200, mainPlayer);
		}
	}


	if (killedPlayer)
	{
		FRotator temp = GetActorRotation();
		FRotator res = FMath::RInterpTo(GetActorRotation(), rotateKill, DeltaTime, killRotTime);
		res.Pitch = temp.Pitch;
		res.Roll = temp.Roll;
		SetActorRotation(res);
	}

	
}

