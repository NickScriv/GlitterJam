// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AxeCanDamage.h"
#include "MainCharacter.h"
#include "Monster.h"
#include "Kismet/GameplayStatics.h"
#include "DestructibleActor.h"
#include "DestructibleComponent.h"
#include "Perception/AISense_Damage.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "GlitterGameModeBase.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"

void UAnimNotify_AxeCanDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	Super::Notify(MeshComp, Animation);

	AMainCharacter* player = Cast<AMainCharacter>(MeshComp->GetOwner());

	if (!player)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_AxeCanDamage: No Player found"));
		return;
	}

	FVector dir;
	FVector start;
	FVector end;

	player->GetCrossHairScreenCoordinates(start, dir);

	TArray<AActor*> actorsToIgnore;

	actorsToIgnore.Add(player);
	FHitResult hit;

	end = (dir * axeRange) + start;

	if (UKismetSystemLibrary::LineTraceSingle(player, start, end, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel6), false, actorsToIgnore, EDrawDebugTrace::None, hit, true))
	{

		if (AActor* actor = hit.GetActor())
		{

			if (actor->ActorHasTag(FName("Monster")))
			{
				AMonster* monster = Cast<AMonster>(actor);

				if (monster)
				{
					FVector hitDir = (monster->GetActorLocation() - player->GetActorLocation()).GetSafeNormal();
					monster->TakeMonsterDamage(20.f, -hit.Normal);
					UAISense_Damage::ReportDamageEvent(player, monster, player, 1.f, player->GetActorLocation(), hit.Location);

					// Play slash blood sound
					FAkAudioDevice::Get()->PostEvent("Axe_Hit_Enemy", MeshComp->GetOwner());

					// Play blood hit effect
					//UGameplayStatics::SpawnEmitterAtLocation(player, monsterHit, hit.Location, UKismetMathLibrary::MakeRotFromZ(dir), false);
				}

			}
			if (ADestructibleActor* wood = Cast<ADestructibleActor>(actor))
			{
				// damage wood
				wood->GetDestructibleComponent()->ApplyDamage(1.f, hit.Location, -hit.Normal, 100);

				// Play slash wood sound
				FAkAudioDevice::Get()->PostEvent("Axe_Hit_Wood", MeshComp->GetOwner());

				// Play wood hit effect?
				UGameplayStatics::SpawnEmitterAtLocation(player, woodHit, hit.Location, UKismetMathLibrary::MakeRotFromZ(dir));
			}
			else if(!actor->ActorHasTag(FName("Monster")))
			{
				// Play slash default sound
				FAkAudioDevice::Get()->PostEvent("Axe_Hit_Generic_Object", MeshComp->GetOwner());

				// Play default hit effect
				UGameplayStatics::SpawnEmitterAtLocation(player, normalHit, hit.Location, UKismetMathLibrary::MakeRotFromZ(dir));
			}
		}
	}
}
