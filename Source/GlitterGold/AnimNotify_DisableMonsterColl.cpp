// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_DisableMonsterColl.h"
#include "Monster.h"
#include "Components/CapsuleComponent.h"

void UAnimNotify_DisableMonsterColl::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AMonster* monster = Cast<AMonster>(MeshComp->GetOwner());

	if (!monster)
		return;

	if (monster->GetMesh())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Sleep bodies"));
		/*monster->GetMesh()->PutRigidBodyToSleep(FName("clavicle_l"));
		monster->GetMesh()->PutRigidBodyToSleep(FName("clavicle_r"));*/
		//monster->GetMesh()->PutAllRigidBodiesToSleep();

		FPhysicalAnimationData data;
		data.OrientationStrength = 25.;
		data.AngularVelocityStrength = 5.;
		data.PositionStrength = 25.;
		data.VelocityStrength = 5.;
		data.bIsLocalSimulation = false;
		data.MaxAngularForce = 0.f;
		data.MaxLinearForce = 0.f;

		monster->physicsComponent->ApplyPhysicalAnimationSettingsBelow(FName("pelvis"), data);
		monster->GetMesh()->SetAllBodiesPhysicsBlendWeight(1.0f);
		//monster->GetMesh()->SetAllBodiesSimulatePhysics(true);
	}
		

	if(monster->GetCapsuleComponent())
		monster->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);


}