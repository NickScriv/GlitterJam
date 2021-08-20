// Fill out your copyright notice in the Description page of Project Settings.


#include "CrowBar.h"
#include "InteractionWidgetComponent.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"
#include "MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Monster.h"
#include "Kismet/GameplayStatics.h"
#include "DestructibleActor.h"
#include "DestructibleComponent.h"
#include "Perception/AISense_Damage.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

// Sets default values
ACrowBar::ACrowBar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	crowBarMesh = CreateDefaultSubobject<USkeletalMeshComponent>("CrowBar Mesh");
	crowBarMesh->SetupAttachment(RootComponent);

}

void ACrowBar::SwingAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("swing crow bar"));
}

/*void ACrowBar::GetPlayer(AMainCharacter* character)
{
	if (character)
		player = character;
}*/

// Called when the game starts or when spawned
void ACrowBar::BeginPlay()
{
	Super::BeginPlay();
	
	if ((interaction = Cast<UInteractionWidgetComponent>(GetComponentByClass(UInteractionWidgetComponent::StaticClass()))) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Interaction is null!!!"));
		return;
	}

	interaction->OnInteract.AddDynamic(this, &ACrowBar::PickedUp);

	/*if ((hitBox = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()))) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Box collition is null in crow bar"));
		return;
	}*/

	//hitBox->OnComponentBeginOverlap.AddDynamic(this, &ACrowBar::HitSomething);
	
}

void ACrowBar::PickedUp(AMainCharacter* character)
{
	character->SpawnCrowBar();
	FAkAudioDevice::Get()->PostEvent("Pickup_Flashlight", this);
	Destroy();
}

/*void ACrowBar::HitSomething(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && player && player->axeCanDamage)
	{
		UE_LOG(LogTemp, Error, TEXT("Axe hit: %s"), *OtherActor->GetClass()->GetName());

		TArray<FHitResult> OutHits;
		FComponentQueryParams Params;
		Params.AddIgnoredActor(this);
		GetWorld()->ComponentSweepMulti(OutHits, hitBox, hitBox->GetComponentLocation() + FVector(-.1f), hitBox->GetComponentLocation() + FVector(0.1f), hitBox->GetComponentRotation(), Params);

		FVector position = FVector::ZeroVector;
		FVector normal = FVector::ZeroVector;
		for (const FHitResult& Result : OutHits)
		{
			if (Result.GetActor() == OtherActor)
			{
				position = Result.ImpactPoint;
				normal = Result.ImpactNormal;
			}
		}

		player->axeCanDamage = false;

		if (OtherActor->ActorHasTag(FName("Monster")))
		{
			AMonster* monster = Cast<AMonster>(OtherActor);

			if (monster)
			{
				FVector hitDir = (monster->GetActorLocation() - player->GetActorLocation()).GetSafeNormal();
				monster->TakeDamage(20.f, -normal);
				UAISense_Damage::ReportDamageEvent(this, monster, player, 1.f, player->GetActorLocation(), position);

				// Play slash blood sound

				// Play blood hit effect

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), monsterHit, position, UKismetMathLibrary::MakeRotFromZ(-normal), false);
			}
			
		}
		else if (ADestructibleActor* wood = Cast<ADestructibleActor>(OtherActor))
		{
			// damage wood
			wood->GetDestructibleComponent()->ApplyDamage(1.f, position, -normal, 100);
			UE_LOG(LogTemp, Error, TEXT("Damage wood"));
			// Play slash wood sound

			// Play wood hit effect?
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), woodHit, position, UKismetMathLibrary::MakeRotFromZ(normal));
		}
		else
		{
			// Play slash default sound

			// Play default hit effect
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), normalHit, position, UKismetMathLibrary::MakeRotFromZ(normal));
		}
	}
}*/

