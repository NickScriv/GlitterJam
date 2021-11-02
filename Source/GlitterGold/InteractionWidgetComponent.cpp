// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWidgetComponent.h"
#include "MainCharacter.h"
#include "InteractionWidget.h"
#include "Kismet/GameplayStatics.h"

UInteractionWidgetComponent::UInteractionWidgetComponent()
{
	SetComponentTickEnabled(false);

	interactionTime = 0.f;
	interactionDistance = 200.f;
	interactableName = FText::FromString("Interactable Object");
	interactableAction = FText::FromString("Interact");

	Space = EWidgetSpace::Screen;
	DrawSize = FIntPoint(600, 100);

	SetActive(true);
	SetHiddenInGame(true);

}

void UInteractionWidgetComponent::BeginFocus()
{
	if (!IsActive() || !GetOwner() || !character)
		return;

	OnBeginFocus.Broadcast(character);

	SetHiddenInGame(false);

	//May get rid of outline
	for (auto & visualComp : GetOwner()->GetComponentsByClass(UPrimitiveComponent::StaticClass()))
	{
		if (UPrimitiveComponent* prim = Cast<UPrimitiveComponent>(visualComp))
		{
			prim->SetRenderCustomDepth(true);
		}
	}

	RefreshWidget();
}

void UInteractionWidgetComponent::EndFocus()
{
	if (!IsActive() || !GetOwner() || !character)
		return;

	OnEndFocus.Broadcast(character);

	SetHiddenInGame(true);

	//May get rid of outline
	for (auto & visualComp : GetOwner()->GetComponentsByClass(UPrimitiveComponent::StaticClass()))
	{
		if (UPrimitiveComponent* prim = Cast<UPrimitiveComponent>(visualComp))
		{
			prim->SetRenderCustomDepth(false);
		}
	}
}

void UInteractionWidgetComponent::BeginInteraction()
{
	//UE_LOG(LogTemp, Warning, TEXT("begin"));
	OnBeginInteract.Broadcast(character);
}

void UInteractionWidgetComponent::EndInteraction()
{
	OnEndInteract.Broadcast(character);
}

void UInteractionWidgetComponent::Interact()
{
	OnInteract.Broadcast(character);
}

float UInteractionWidgetComponent::GetInteractPercent()
{
	if(FMath::IsNearlyEqual(character->GetRemainingInteractTime(), -1.f, 0.1f))
		return 0.f;
	return 1.f - FMath::Abs(character->GetRemainingInteractTime() / interactionTime);
}

void UInteractionWidgetComponent::SetInteractableNameText(const FText& newName)
{
	interactableName = newName;
	RefreshWidget();
}

void UInteractionWidgetComponent::SetInteractableActionText(const FText& newAction)
{
	interactableAction = newAction;
	RefreshWidget();

}

void UInteractionWidgetComponent::SetInteractableKeyAction(const FText& newKeyAction)
{
	interactableKeyAction = newKeyAction;
	RefreshWidget();
}

void UInteractionWidgetComponent::RefreshWidget()
{
	if (!bHiddenInGame)
	{
		if (UInteractionWidget* interactionWidget = Cast<UInteractionWidget>(GetUserWidgetObject()))
		{
			interactionWidget->UpdateInteractionWidget(this);
		}
	}
}

bool UInteractionWidgetComponent::CanInteract()
{
	return IsActive() && GetOwner() && character;
}

void UInteractionWidgetComponent::Deactivate()
{
	EndFocus();
	EndInteraction();
}



// Called when the game starts
void UInteractionWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	character = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (!character)
	{
		//UE_LOG(LogTemp, Error, TEXT("Character not found in Interactable component!!"));
	}

}



