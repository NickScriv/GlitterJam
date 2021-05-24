// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWidget.h"
#include "InteractionWidgetComponent.h"

void UInteractionWidget::UpdateInteractionWidget(UInteractionWidgetComponent* interactionComponent)
{
	owningInteractionComponent = interactionComponent;
	OnUpdateInteractWidget();
}