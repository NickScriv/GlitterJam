// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotesUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GLITTERGOLD_API UNotesUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
		UFUNCTION(BlueprintImplementableEvent)
		void SetNoteActor(class ANoteActor* noteActor);

		UFUNCTION(BlueprintImplementableEvent)
		void ExitPressed();

		UFUNCTION(BlueprintImplementableEvent)
		void TextPressed();
	
};
