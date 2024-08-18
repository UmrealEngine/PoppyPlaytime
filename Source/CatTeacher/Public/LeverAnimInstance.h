// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KHH_EnemyFSM.h"
#include "LeverAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CATTEACHER_API ULeverAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

	//UPROPERTY()
	//class AKHH_Enemy* enemy;

	//UPROPERTY(EditDefaultsOnly)
	//class UAnimMontage* EnemyMontage;
	//
	// 상태, 공격여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EEnemyState DelightState;

	//UFUNCTION()
	//void AimNotify_MoveStart();

	///*UFUNCTION()
	//void AimNotify_MoveEnd();*/

	//UFUNCTION()
	//void AimNotify_RunStart();

	////UFUNCTION()
	////void AimNotify_RunEnd();// die

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FSMComponent)
	//bool LeverMove = false ;



};
