// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_PlayerHandFSM.h"
#include "KMK_Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "KMK_Bullet.h"
#include "KMK_PlayerHand.h"
#include "PlayerAnimInstance.h"
#include "../../../../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

// Sets default values for this component's properties
UKMK_PlayerHandFSM::UKMK_PlayerHandFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UKMK_PlayerHandFSM::BeginPlay()
{
	Super::BeginPlay();
	// 플레이어 객체 들고오기
	Player = Cast<AKMK_Player>(GetWorld()->GetFirstPlayerController()->GetPawn());
	movementComp = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
	JumpPower = movementComp->JumpZVelocity;
}


// Called every frame
void UKMK_PlayerHandFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PState != PlayerHandFSM::Energy && !isCharge)
	{
		Player->CableComp[0]->SetMaterial(0, cableMat[0]);
		Player->Hands[0]->VFXComp->SetVisibility(false);
		Player->Hands[0]->myMatDynamic->SetScalarParameterValue("charge_light", 0);
		if (Player->audioComps[6]->IsPlaying())
		{
			Player->StopPlayerSound(6);
		}
	}
	// 스테이트 변경
	switch (PState)
	{
		case PlayerHandFSM::Normal:
			NormalHand();
		break;
		case PlayerHandFSM::JumpPack:
			JumpHand();
		break;
		case PlayerHandFSM::GunPack:
			GunHand();
		break;
		case PlayerHandFSM::Energy:
			EnergyHand();
		break;
	}
}

#pragma region Normal
void UKMK_PlayerHandFSM::NormalHand()
{

	GEngine->AddOnScreenDebugMessage(4, 1, FColor::Blue, FString::Printf(TEXT("remove")));
}
#pragma endregion
#pragma region Jump
// 일정 트리거가 발견되면 점프력을 2배로
void UKMK_PlayerHandFSM::JumpHand()
{

	if(isJump)
	{
		Player->isSFX[0] = false;
		Player->isSFX[1] = false;
		if(movementComp->JumpZVelocity != JumpPower * 2)
		{
			movementComp->JumpZVelocity = JumpPower * 1.8f;
			Player->Jump();
			isJump = false;
		}
	}
	else
	{
		movementComp->JumpZVelocity = JumpPower;
	}
}
#pragma endregion
#pragma region Gun
void UKMK_PlayerHandFSM::GunHand()
{
	if (isFire && cnt < 1)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Player->soundCue[8], Player->GetActorLocation());
		// 총알 효과 재생
		GetWorld()->SpawnActor<AKMK_Bullet>(bulletFact, bulletTrans);
		Player->anim->PlayFireMontage();
		cnt++;
	}
}
#pragma endregion
#pragma region Energy
void UKMK_PlayerHandFSM::EnergyHand()
{
	Player->CableComp[0]->SetMaterial(0, cableMat[1]);
	t += GetWorld()->DeltaTimeSeconds;
	if (!Player->audioComps[6]->IsPlaying())
	{
		Player->PlayPlayerSound(6);
	}
    GEngine->AddOnScreenDebugMessage(8, 1, FColor::Blue, FString::Printf(TEXT("charge")));
	if (t > chargeTime)
	{
		isCharge = false;
		if (Player->audioComps[6]->IsPlaying())
		{
			Player->StopPlayerSound(6);
		}
		Player->CableComp[0]->SetMaterial(0, cableMat[0]);
		PState = PlayerHandFSM::Normal;
		t = 0;
	}
}
#pragma endregion
