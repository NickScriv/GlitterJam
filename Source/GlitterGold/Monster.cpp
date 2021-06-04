// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMonster::KillPlayer()
{
	AAIController* controller =  UAIBlueprintHelperLibrary::GetAIController(this);

	if (controller)
	{
		AMainCharacter* player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (player)
		{
			player->Died(this);
			controller->SetFocus(player);

			FVector playerLookAt = GetActorLocation();
			rotateKill= UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), player->GetActorLocation());

			killedPlayer = true;
		}
		controller->GetBrainComponent()->StopLogic("Dead");
	}

	GetCharacterMovement()->DisableMovement();
	

}

void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (killedPlayer)
	{
		FRotator temp = GetActorRotation();
		FRotator res = FMath::RInterpTo(GetActorRotation(), rotateKill, DeltaTime, killRotTime);
		res.Pitch = temp.Pitch;
		res.Roll = temp.Roll;
		SetActorRotation(res);
	}
	
}

