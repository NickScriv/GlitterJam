// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerInRange.h"
#include "Monster.h"
#include "MainCharacter.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"

UBTService_PlayerInRange::UBTService_PlayerInRange()
{
	bNotifyBecomeRelevant = 1;
}

void UBTService_PlayerInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp,  NodeMemory);
}

void UBTService_PlayerInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	////UE_LOG(LogTemp, Warning, TEXT("wow"));
	if (!OwnerComp.GetAIOwner())
		return;

	AMonster* monster = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!monster)
		return;

	AMainCharacter* player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (!player)
		return;

	FCollisionQueryParams qParams;

	FHitResult hit;
	FVector start = monster->GetActorLocation();
	qParams.AddIgnoredActor(monster);
	start.Z += 130.f;
	//UE_LOG(LogTemp, Warning, TEXT("check player"));
	if (GetWorld()->LineTraceSingleByChannel(hit, start, player->GetCapsuleComponent()->GetComponentLocation(), ECC_Visibility, qParams))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit something"));
		if (Cast<AMainCharacter>(hit.Actor) && hit.Distance <= killDistance)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Kill Player"));
			//monster->StopMonsterSounds();
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("PlayerInRange", true);
		}
	}
	//DrawDebugLine(GetWorld(), start, player->GetCapsuleComponent()->GetComponentLocation(), FColor::Yellow, false, 2.f);
	//UE_LOG(LogTemp, Warning, TEXT("check player 2"));
	//UE_LOG(LogTemp, Warning, TEXT("check player 3"));
}


