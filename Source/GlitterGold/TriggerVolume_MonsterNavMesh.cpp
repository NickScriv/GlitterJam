// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerVolume_MonsterNavMesh.h"
#include "Monster.h"
#include "MainCharacter.h"
#include "Components/ArrowComponent.h"
#include "NavModifierVolume.h"
#include "AI/NavigationSystemBase.h"

ATriggerVolume_MonsterNavMesh::ATriggerVolume_MonsterNavMesh()
{
	arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	arrow->SetupAttachment(RootComponent);
}

void ATriggerVolume_MonsterNavMesh::BeginPlay()
{
	Super::BeginPlay();

	// listen to event
	OnActorBeginOverlap.AddUniqueDynamic(this, &ATriggerVolume_MonsterNavMesh::OnOverlapNavBegin);
	OnActorEndOverlap.AddUniqueDynamic(this, &ATriggerVolume_MonsterNavMesh::OnOverlapNavEnd);
	//DrawDebugBox(GetWorld(), GetActorLocation(), Brush->Bounds.BoxExtent, FColor::Orange, true, -1, 0, 5);
}

void ATriggerVolume_MonsterNavMesh::OnOverlapNavBegin(AActor* overlappedActor, AActor* otherActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("monster Begin Overlap"));
	if (otherActor && otherActor != this)
	{
		AMonster* monster = Cast<AMonster>(otherActor);
		//UE_LOG(LogTemp, Warning, TEXT("check monster"));
		if (!monster)
		{
			//UE_LOG(LogTemp, Error, TEXT("Monster Not found in trigger"));
			return;
		}
		//UE_LOG(LogTemp, Warning, TEXT("overlap monster"));
		for (ANavModifierVolume* navMod : downStairMeshVolumes)
		{

			navMod->SetAreaClass(defaultNavAreaClass);
			//UE_LOG(LogTemp, Warning, TEXT("Set default nav modifier"));
			
		}

		for (ANavModifierVolume* navMod : upStairMeshVolumes)
		{
			navMod->SetAreaClass(defaultNavAreaClass);
			//UE_LOG(LogTemp, Warning, TEXT("Set default nav modifier"));
		}
	}
}

void ATriggerVolume_MonsterNavMesh::OnOverlapNavEnd(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		AMonster* monster = Cast<AMonster>(otherActor);

		if (!monster)
		{
			//UE_LOG(LogTemp, Error, TEXT("Monster Not found in trigger"));
			return;
		}

		FVector toMonster = monster->GetActorLocation() - GetActorLocation();
		float dot = FVector::DotProduct(toMonster, arrow->GetForwardVector());

		if (dot > 0)
		{
			// Downstairs, so disable up stair navmeshes
			for (ANavModifierVolume* navMod : upStairMeshVolumes)
			{
				navMod->SetAreaClass(nullNavAreaClass);
				//UE_LOG(LogTemp, Warning, TEXT("Set null nav modifier"));
				
			}
		}
		else
		{
			// Upstairs, so disable down stair navmeshes
			for (ANavModifierVolume* navMod : downStairMeshVolumes)
			{

				navMod->SetAreaClass(nullNavAreaClass);
				//UE_LOG(LogTemp, Warning, TEXT("Set null nav modifier"));
				
			}

		}

	}
}
