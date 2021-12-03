// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrowBar.generated.h"


UCLASS()
class GLITTERGOLD_API ACrowBar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrowBar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* crowBarMesh;

	UPROPERTY(EditDefaultsOnly)
		class UInteractionWidgetComponent* interaction;

	UFUNCTION()
		void PickedUp(class AMainCharacter* character);

};
