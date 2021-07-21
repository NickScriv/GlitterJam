// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FinalDoor.generated.h"

UCLASS()
class GLITTERGOLD_API AFinalDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFinalDoor();
	
	void InitInteraction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	

	UPROPERTY()
		class UInteractionWidgetComponent* interaction;

	UFUNCTION()
	void TriggerEnd(class AMainCharacter* character);

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> interarctionWidgetClass;

};
