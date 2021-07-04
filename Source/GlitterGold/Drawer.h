// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Drawer.generated.h"

UCLASS()
class GLITTERGOLD_API ADrawer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrawer();

	bool IsOpening();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	
	UPROPERTY()
		class UInteractionWidgetComponent* interaction = nullptr;

	UFUNCTION()
		void InteractDrawer(class AMainCharacter* character);

	UFUNCTION()
		void BeginFocusDrawer(class AMainCharacter* character);

	UFUNCTION()
		void TimelineProgressOpenClose(float val);

	UFUNCTION()
		void FinishOpenClose();

	bool open = false;

	bool opening = false;

	FVector startLoc;
	
	FVector endLoc;

	UPROPERTY(EditAnywhere)
	float distanceToOpen;

	FTimeline openCloseTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* openCloseCurveFloat;

	void OpenCloseDrawer();
};
