// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateInteractWidget();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void UpdateInteractionWidget(class UInteractionWidgetComponent* interactionComponent);

	UPROPERTY(BlueprintReadOnly, Category = "Interaction", meta= (ExposeOnSpawn))
	class UInteractionWidgetComponent* owningInteractionComponent;



	
};
