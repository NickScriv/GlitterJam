// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "PathPoint.h"
#include "DoorOpenClose.generated.h"

USTRUCT()
struct FPathPointArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		TArray<APathPoint*> pathPoints;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLITTERGOLD_API UDoorOpenClose : public USceneComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UDoorOpenClose();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor();

	UPROPERTY(BlueprintReadOnly)
	float openTime = 2;

	UPROPERTY(EditAnywhere)
	bool locked = false;

	bool playerHasKey = false;

	bool playerHasLockPick = false;

private:

	UPROPERTY(EditAnywhere, meta = (EditCondition = locked))
		class ANavModifierVolume* navMeshToBlock;

	UPROPERTY(EditAnywhere, meta = (EditCondition = locked))
		TSubclassOf<UNavArea> defaultNavAreaClass;

	UPROPERTY(EditAnywhere)
	float openAngle;

	UPROPERTY(EditAnywhere, meta = (EditCondition = locked))
	float unlockDoorTime = 0.f;

	UPROPERTY(EditAnywhere, meta = (EditCondition = locked))
	float unlockLockPickTime;

	UPROPERTY(EditAnywhere, meta = (EditCondition = locked))
	class AKeyActor* keyToDoor;

	UPROPERTY(EditAnywhere, meta = (EditCondition = locked))
	class ALockPick* lockPick;

	UPROPERTY(EditAnywhere)
	class ANavLinkProxy* navLinkProxyEnter;

	UPROPERTY(EditAnywhere)
	class ANavLinkProxy* navLinkProxyExit;

	float initialYaw;
	float currentYaw;

	UPROPERTY(EditAnywhere)
	float doorSpeed = 0.6f;

	UPROPERTY(EditAnywhere)
	bool open = false;

	UPROPERTY(EditAnywhere)
		class UAkAudioEvent* in_pEventOpen;

	UPROPERTY(EditAnywhere)
		class UAkAudioEvent* in_pEventClose;

	UPROPERTY(EditAnywhere, Category = "MonsterPathing", meta = (EditCondition = locked))
		TArray<FPathPointArray> possibleMonsterPaths;

	UPROPERTY()
	class AGlitterGameModeBase* gameMode;

	FAIRequestID currentMove;
	
	bool checkClosed = false;

	class AMonsterAIController* monsterController = nullptr;

	class AMonster* monster = nullptr;

	UPROPERTY()
		class UInteractionWidgetComponent* interaction = nullptr;

	UPROPERTY()
		class UAkPortalComponent* portal = nullptr;

	UFUNCTION()
		void InteractDoor(class AMainCharacter* character);

	UFUNCTION()
		void BeginFocusDoor(class AMainCharacter* character);

	UFUNCTION()
		void BeginInteractDoor(class AMainCharacter* character);

	UFUNCTION()
		void EndFocusDoor(class AMainCharacter* character);

	UFUNCTION()
		void EndInteractDoor(class AMainCharacter* character);

	UFUNCTION()
		void PlayerPickedUpKey();

	UFUNCTION()
		void PlayerPickedUpLockPick();

	UFUNCTION()
		void LockPickUsed();

	UFUNCTION()
	void MonsterReachedNavLink(AActor* MovingActor, const FVector& DestinationPoint);

	void UnlockNavMesh();

	void ChangeMonsterPath();

};
