// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoteActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickedUpNote);

UCLASS()
class GLITTERGOLD_API ANoteActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoteActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* noteMesh;

	UPROPERTY(EditDefaultsOnly)
		class UInteractionWidgetComponent* interaction;

	UPROPERTY(EditAnywhere, Category="Widget")
		class UNotesUserWidget* noteWidget;

	UFUNCTION()
	void PickedUpNote(class AMainCharacter* character);

	void BindToInput();

	UFUNCTION(BlueprintCallable)
	void CloseNote(class AMainCharacter* character);

	UFUNCTION()
		void TextPressed();

	UFUNCTION()
		void ExitPressed();

	UFUNCTION()
		void CanExitNoteExpire();
	FTimerHandle exitNoteTimer;

	bool canExit = true;

	float timeElapsed = 0.f;

	UPROPERTY(EditAnywhere)
	float delayTimer = 0.5f;


};
