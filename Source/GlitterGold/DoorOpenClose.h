// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkGameplayStatics.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkComponent.h"
#include "DoorOpenClose.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLITTERGOLD_API UDoorOpenClose : public UAkComponent
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

private:
	UPROPERTY(EditAnywhere)
	float openAngle = 90.f;

	float initialYaw;
	float currentYaw;

	int32 openSoundID;

	UPROPERTY()
		UAudioComponent* audioComponent = nullptr;

	UPROPERTY(EditAnywhere)
		float doorSpeed = 0.8f;



	

	UPROPERTY(EditAnywhere)
	bool open = false;

	UPROPERTY(EditAnywhere)
		class UInteractionWidgetComponent* interaction = nullptr;

	UFUNCTION()
		void InteractDoor(class AMainCharacter* character);

	UFUNCTION()
		void FocusDoor(class AMainCharacter* character);

		
};
