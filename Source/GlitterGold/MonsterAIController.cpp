// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainCharacter.h"
#include "Monster.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Prediction.h"
#include "Perception/AISense_Touch.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AIHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"


void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	monster = Cast<AMonster>(GetPawn());

	if (!AIBlackboard)
	{
		return;
	}

	UseBlackboard(AIBlackboard, blackboardComp);

	if (!AIBehavior)
	{
		return;
	}
	
	TArray<UAIPerceptionComponent*> perceptionComponents;
	GetComponents<UAIPerceptionComponent>(perceptionComponents);

	for(UAIPerceptionComponent* perc : perceptionComponents)
	{

		if(perc->GetName() == "AIPerception2")
		{
			//UE_LOG(LogTemp, Warning, TEXT("AIPerception2 found!"));
			AIPerception = perc;
			break;
		}
	}
	
	blackboardComp->SetValueAsInt(FName("ChangePath"), 0);
	blackboardComp->SetValueAsObject(FName("TargetActor"), player);
	blackboardComp->SetValueAsEnum(FName("MonsterStatus"), (uint8)MonsterStatus::Patrolling); 

	predictNavHelper = GetWorld()->SpawnActor<AActor>(predictNavClass);
	playerPredictNavHelper = GetWorld()->SpawnActor<AActor>(predictNavClass);

	currentSightRange = sightRange;
	currentLoseSightRange = loseSightRange;
	detectRange = currentSightRange;

	currentSightAngle = sightHalfAngleDegrees;
	
}

void AMonsterAIController::perceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Cast<AMainCharacter>(Actor))
		return;

	FString senseName = UKismetSystemLibrary::GetDisplayName(UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus));
	if (senseName == TEXT("AISense_Hearing") && blackboardComp->GetValueAsEnum(FName("MonsterStatus")) != (uint8)MonsterStatus::Chasing && canHear)
	{

		FHitResult hit;
		FCollisionQueryParams qParams;
		qParams.AddIgnoredActor(monster);

		if (!GetWorld())
			return;
		//UE_LOG(LogTemp, Warning, TEXT("Hear!!!!!!!!!!"));
		if (GetWorld()->LineTraceSingleByChannel(hit, monster->GetActorLocation(), Stimulus.StimulusLocation, ECC_Visibility, qParams))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Hit raycast to player: %s"), *hit.Actor->GetName());
			if (Cast<AMainCharacter>(hit.Actor))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Heard Player at: %f, %f, %f"), Stimulus.StimulusLocation.X, Stimulus.StimulusLocation.Y, Stimulus.StimulusLocation.Z);
				if (Stimulus.WasSuccessfullySensed())
				{
					SetInvestigatingBlackboard();
					blackboardComp->SetValueAsVector(FName("TargetLoc"), Stimulus.StimulusLocation);
				}
				/*else
				{
					blackboardComp->SetValueAsInt(FName("IsInvestigating"), Stimulus.WasSuccessfullySensed());
				}*/
				
			}
		}
	}
	else if (senseName == TEXT("AISense_Damage") && blackboardComp->GetValueAsEnum(FName("MonsterStatus")) != (uint8)MonsterStatus::Chasing)
	{

		//UE_LOG(LogTemp, Warning, TEXT("Damage AI at: %f, %f, %f"), Stimulus.StimulusLocation.X, Stimulus.StimulusLocation.Y, Stimulus.StimulusLocation.Z);
		if (Stimulus.WasSuccessfullySensed())
		{
			SetInvestigatingBlackboard();
			blackboardComp->SetValueAsVector(FName("TargetLoc"), Stimulus.StimulusLocation);
		}
		

	}
	else if (senseName == TEXT("AISense_Prediction"))
	{

		//UE_LOG(LogTemp, Warning, TEXT("AI Predict!!"));
		predictLoc = Stimulus.StimulusLocation;
		predictNavHelper->SetActorLocation(Stimulus.StimulusLocation);
		playerPredictNavHelper->SetActorLocation(player->GetActorLocation());
	}
	else if (senseName == TEXT("AISense_Touch") && blackboardComp->GetValueAsEnum(FName("MonsterStatus")) != (uint8)MonsterStatus::Chasing && !isScreaming)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Sensed player touch"));
		if (Stimulus.WasSuccessfullySensed())
		{
			SetInvestigatingBlackboard();
			blackboardComp->SetValueAsVector(FName("TargetLoc"), Stimulus.StimulusLocation);
			
		}
		
	}
}

void AMonsterAIController::StopScreaming()
{
	blackboardComp->SetValueAsBool(FName("IsScreaming"), false);
	isScreaming = false;
	monster->GetCharacterMovement()->Activate();

	if (chasing)
	{
		currentSightAngle = loseSightHalfAngleDegrees;
		FAkAudioDevice::Get()->PostEvent("Play_Enemy_Chase_Sounds", monster);
	}
	else
	{
		FAkAudioDevice::Get()->PostEvent("Play_Enemy_Caution_Sounds", monster);
	}
}


void AMonsterAIController::SetToDefaultPerception()
{
	//UKismetSystemLibrary::PrintString(this, FString("SetToDefaultPerception"));
	if (!firstSeen && lostSight)
	{
		firstSeen = true;
		currentSightRange = mainSightRange;
		currentLoseSightRange = mainLoseSightRange;
		detectRange = currentSightRange;
	}
}

void AMonsterAIController::SetInvestigatingBlackboard()
{
	if (blackboardComp->GetValueAsInt(FName("IsInvestigating")) == 0)
	{
		blackboardComp->SetValueAsInt(FName("IsInvestigating"), 1);
	}
	else if (blackboardComp->GetValueAsInt(FName("IsInvestigating")) == 1)
	{
		blackboardComp->SetValueAsInt(FName("IsInvestigating"), 2);
	}
	else
	{
		blackboardComp->SetValueAsInt(FName("IsInvestigating"), 1);
	}
}

bool AMonsterAIController::PlayerLineCheck(bool checkCone)
{
	TArray<AActor*> ignoreActors;
	FHitResult HitResult;
	ignoreActors.Add(monster);

	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(this, monster->GetMesh()->GetSocketTransform("Eyes").GetLocation(), player->GetActorLocation(),
		5.0f, UEngineTypes::ConvertToTraceType(ECC_Visibility)
		, false, ignoreActors, EDrawDebugTrace::None, HitResult, true);

	if (!bHit || !Cast<AMainCharacter>(HitResult.Actor))
	{
		//UKismetSystemLibrary::PrintString(this, FString("Player Hit Line"));
		return false;
	}

	if (checkCone)
	{
		if (HitResult.Distance > detectRange)
			return false;

		FVector facingDirection = monster->GetMesh()->GetSocketTransform("Eyes").GetRotation().GetForwardVector();
		FVector dir = (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal();
		float dotRes = FVector::DotProduct(dir, facingDirection);
		float angle = UKismetMathLibrary::DegAcos(dotRes);

		if (angle > currentSightAngle)
			return false;

	}
	//UKismetSystemLibrary::PrintString(this, FString("Player NOT Hit Line"));
	return true;
}

void AMonsterAIController::HandleSight(bool successfullySensed)
{
	if (successfullySensed)
	{
		currentSightAngle = loseSightHalfAngleDegrees;
		
		if (blackboardComp->GetValueAsEnum(FName("MonsterStatus")) == (uint8)MonsterStatus::Patrolling)
		{
			blackboardComp->SetValueAsBool(FName("IsScreaming"), true);
			isScreaming = true;
			//FAkAudioDevice::Get()->PostEventAtLocation("Player_Spotted", monster);
			FAkAudioDevice::Get()->PostEventAtLocation("Player_Spotted", monster->GetActorLocation(), monster->GetActorRotation(), GetWorld());
			monster->GetCharacterMovement()->Deactivate();
			currentSightAngle = screamingSightHalfAngleDegrees;
		}
		blackboardComp->SetValueAsEnum(FName("MonsterStatus"), (uint8)MonsterStatus::Chasing);
		blackboardComp->SetValueAsInt(FName("IsInvestigating"), 0);

		//UE_LOG(LogTemp, Warning, TEXT("Monster sight sensed"));
		chasing = true;
	}
	else
	{
		if(!firstSeen)
			SetToDefaultPerception();

		currentSightAngle = sightHalfAngleDegrees;
		chasing = false;
		canHear = false;
		blackboardComp->SetValueAsBool(FName("IsPredicting"), true);
		UAISense_Prediction::RequestControllerPredictionEvent(this, player, predictionTime);
	}

	blackboardComp->SetValueAsBool(FName("CanSeePlayer"), successfullySensed);
}

void AMonsterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	//UE_LOG(LogTemp, Warning, TEXT("Tick!!!"));

		if (inSightCone && !PlayerLineCheck(true))
		{
			// Lost player
			inSightCone = false;
			lostSight = true;
			//UKismetSystemLibrary::PrintString(this, FString("Sight failed"));
			detectRange = currentSightRange;
			HandleSight(false);
		}
		else if (!inSightCone && PlayerLineCheck(true))
		{
			// See Player
			inSightCone = true;
			lostSight = false;
			//UKismetSystemLibrary::PrintString(this, FString("Sight success"));
			detectRange = currentLoseSightRange;
			HandleSight(true);
		}

	//UE_LOG(LogTemp, Error, TEXT("sight angle: %f"), currentSightAngle);
}

void AMonsterAIController::StartMonsterBehavior()
{
	//UE_LOG(LogTemp, Error, TEXT("Start monster behavior"));
	// sanity check
	if (startedBehavior)
		return;

	startedBehavior = true;

	if (AIPerception)
	{
		AIPerception->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AMonsterAIController::perceptionUpdated);
	}

	RunBehaviorTree(AIBehavior);

	SetActorTickEnabled(true);
	//SetActorTickInterval(0.05);
	GetWorld()->GetTimerManager().SetTimer(timerHandleFirstSeen, this, &AMonsterAIController::SetToDefaultPerception, ChangePerceptionTime, false);
}

void AMonsterAIController::TriggerPatrolAbort()
{
	// Just need to trigger abort without conditional
	if(blackboardComp->GetValueAsEnum(FName("MonsterStatus")) == (uint8)MonsterStatus::Patrolling)
		StopMovement();

	if (changePathValue)
	{
		blackboardComp->SetValueAsInt(FName("ChangePath"), blackboardComp->GetValueAsInt(FName("ChanePath")) + 1);
	}
	else
	{
		blackboardComp->SetValueAsInt(FName("ChangePath"), blackboardComp->GetValueAsInt(FName("ChanePath")) - 1);
	}

	changePathValue = !changePathValue;
}

FVector AMonsterAIController::GetPredictLoc()
{
	//UE_LOG(LogTemp, Warning, TEXT("Return Loc: %f, %f, %f"), predictLoc.X, predictLoc.Y, predictLoc.Z);
	return predictLoc;
}
