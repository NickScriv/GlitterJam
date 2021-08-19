// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"
#include "Monster.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"
#include "InteractionWidgetComponent.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AISense_Damage.h"

// Sets default values
AShotgun::AShotgun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Shotgun Mesh");
	mesh->SetupAttachment(RootComponent);

}

void AShotgun::Shoot()
{
	UGameplayStatics::SpawnEmitterAttached(muzzleFlash, mesh, TEXT("Shoot"));
	UGameplayStatics::SpawnEmitterAttached(muzzleSmoke, mesh, TEXT("Shoot"));

	AMainCharacter* player = Cast<AMainCharacter>(GetOwner());

	if (!player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player not found in shotgun!!"));
		return;
	}

	FVector dir;
	FVector start;

	if (!player->isAiming)
	{
		if (player->GetController())
		{
			FRotator viewPointRot;
			FVector viewPointLoc;
			player->GetController()->GetPlayerViewPoint(viewPointLoc, viewPointRot);

			dir = viewPointRot.Vector();
			start = viewPointLoc;
			start.Z -= hipOffset;
		}

	}
	else
	{
		FRotator rot = mesh->GetSocketRotation(TEXT("Shoot"));
		dir = FRotationMatrix(rot).GetScaledAxis(EAxis::X);
		start = mesh->GetSocketLocation(TEXT("Shoot"));
		start.X -= aimOffset;
	}


	for (int32 i = 0; i < pellets; i++)
	{
		FVector shotDir = UKismetMathLibrary::RandomUnitVectorInConeInRadians(dir, coneRadius);
		FVector shot = shotDir * range;

		FCollisionQueryParams qParams;
		qParams.AddIgnoredActor(this);
		qParams.AddIgnoredActor(player);

		FHitResult hit;

		FVector end = start + shot;

		if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, qParams))
		{
			if (AMonster* monster = Cast<AMonster>(hit.Actor))
			{
				// Hit monster
				UGameplayStatics::SpawnEmitterAtLocation(this, fleshImpact, hit.Location, hit.Normal.Rotation());
				monster->TakeDamage(1.0, shotDir);
				UAISense_Damage::ReportDamageEvent(this, monster, player, 1.f, start, hit.Location);
			}
			else
			{
				// Hit something else
				UGameplayStatics::SpawnEmitterAtLocation(this, normalImpact, hit.Location, (-shotDir).Rotation());
			}
		}
	}
}

// Called when the game starts or when spawned
void AShotgun::BeginPlay()
{
	Super::BeginPlay();

	if ((interaction = Cast<UInteractionWidgetComponent>(GetComponentByClass(UInteractionWidgetComponent::StaticClass()))) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Interaction is null!!!"));
		return;
	}

	interaction->OnInteract.AddDynamic(this, &AShotgun::PickedUp);
	
}

void AShotgun::PickedUp(AMainCharacter* character)
{
	character->SpawnShotgun();
	FAkAudioDevice::Get()->PostEvent("Pickup_Flashlight", this);
	Destroy();
}
