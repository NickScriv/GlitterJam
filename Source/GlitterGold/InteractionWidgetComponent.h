// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractionWidgetComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginInteract, class AMainCharacter*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndInteract, class AMainCharacter*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginFocus, class AMainCharacter*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndFocus, class AMainCharacter*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, class AMainCharacter*, character);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GLITTERGOLD_API UInteractionWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
		UInteractionWidgetComponent();

public:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		float interactionTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		float interactionDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		FText interactableName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		FText interactableAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		FText interactableKeyAction = FText::FromString("");

	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
		FOnBeginFocus OnBeginFocus;

	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
		FOnEndFocus OnEndFocus;

	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
		FOnBeginInteract OnBeginInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
		FOnEndInteract OnEndInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
		FOnInteract OnInteract;

	void BeginFocus();
	void EndFocus();

	void BeginInteraction();
	void EndInteraction();

	void Interact();

	UFUNCTION(BlueprintPure, Category = "Interaction")
		float GetInteractPercent();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void SetInteractableNameText(const FText& newName);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void SetInteractableActionText(const FText& newAction);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void SetInteractableKeyAction(const FText& newKeyAction);

	void RefreshWidget();

	bool CanInteract();

	virtual void Deactivate() override;

private:
	class AMainCharacter* character; 
	
};
