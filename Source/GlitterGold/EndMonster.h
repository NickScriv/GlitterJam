// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndMonster.generated.h"

UCLASS()
class GLITTERGOLD_API AEndMonster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndMonster();

	UPROPERTY(BlueprintReadOnly)
	bool startSwiping;


	class USkeletalMeshComponent* GetMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* root;


};
