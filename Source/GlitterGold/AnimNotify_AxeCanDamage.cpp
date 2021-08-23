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

void UAnimNotify_AxeCanDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UE_LOG(LogTemp, Warning, TEXT("AxeDamage 1"));
	Super::Notify(MeshComp, Animation);
	UE_LOG(LogTemp, Warning, TEXT("AxeDamage 2"));
	AMainCharacter* player = Cast<AMainCharacter>(MeshComp->GetOwner());

	if (!player)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_AxeCanDamage: No Player found"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("AxeDamage 3"));
	FVector dir;
	FVector start;
	FVector end;

	player->GetCrossHairScreenCoordinates(start, dir);

	/*if (player->GetController())
	{
		FRotator viewPointRot;
		FVector viewPointLoc;
		
		player->GetController()->GetPlayerViewPoint(viewPointLoc, viewPointRot);

		dir = viewPointRot.Vector();
	}
	else
	{
		return;
	}*/

	TArray<AActor*> actorsToIgnore;

	actorsToIgnore.Add(player);
	FHitResult hit;

	end = (dir * axeRange) + start;

	UE_LOG(LogTemp, Warning, TEXT("AxeDamage 4"));
	if (UKismetSystemLibrary::LineTraceSingle(player, start, end, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore, EDrawDebugTrace::None, hit, true))
	{
		UE_LOG(LogTemp, Warning, TEXT("AxeDamage 5"));
		if (AActor* actor = hit.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("AxeDamage 6"));
			if (actor->ActorHasTag(FName("Monster")))
			{
				AMonster* monster = Cast<AMonster>(actor);

				if (monster)
				{
					FVector hitDir = (monster->GetActorLocation() - player->GetActorLocation()).GetSafeNormal();
					monster->TakeMonsterDamage(20.f, -hit.Normal);
					UAISense_Damage::ReportDamageEvent(player, monster, player, 1.f, player->GetActorLocation(), hit.Location);

					// Play slash blood sound

					// Play blood hit effect

					UGameplayStatics::SpawnEmitterAtLocation(player, monsterHit, hit.Location, UKismetMathLibrary::MakeRotFromZ(dir), false);
				}

			}
			else if (ADestructibleActor* wood = Cast<ADestructibleActor>(actor))
			{
				// damage wood
				wood->GetDestructibleComponent()->ApplyDamage(1.f, hit.Location, -hit.Normal, 100);
				UE_LOG(LogTemp, Error, TEXT("Damage wood"));
				// Play slash wood sound

				// Play wood hit effect?
				UGameplayStatics::SpawnEmitterAtLocation(player, woodHit, hit.Location, UKismetMathLibrary::MakeRotFromZ(dir));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("AxeDamage 7"));
				// Play slash default sound

				// Play default hit effect
				UGameplayStatics::SpawnEmitterAtLocation(player, normalHit, hit.Location, UKismetMathLibrary::MakeRotFromZ(dir));
			}
		}
	}
}
