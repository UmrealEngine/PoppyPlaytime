// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CATTEACHER_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public :
	UPROPERTY()
	class AKMK_Player* player;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Speed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Spd;
	virtual void NativeUpdateAnimation(float DeltaSeconds);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isGrab = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isReverse= false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isChangeHand = false;

	UPROPERTY(EditDefaultsOnly)
	TArray<class UAnimMontage*> Monts;

	void PlayFireMontage();
	void PlayHandMontage();
	void PlayHandInMontage();
	void PlayChangeMontage();

	UFUNCTION()
	void AnimNotify_FireEnd();
	UFUNCTION()
	void AnimNotify_ChangeHand();
	int clickCount = 0;
	UPROPERTY(EditAnywhere, Category = "Hand")
	TArray<class UStaticMesh*> HandMesh;
	UPROPERTY()
	class UStaticMeshComponent* meshComp;

	bool isNormal = false;

	bool isF = false;
};
