// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LeverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API ULeverComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	ULeverComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	FRotator OpenAngle = FRotator(0, 0, 180);
	
	FRotator InitialRotation;

	UPROPERTY(EditAnywhere)
	AActor* Targetshutter;
	void SetTargetShutter(AActor* NewTargetShutter);

	UPROPERTY()
	class UKHH_BossOpendoor* BossOpendoorComponent;


	UPROPERTY(EditAnywhere)
	float MoveTime = 1;

	/*UPROPERTY(EditAnywhere)
	class ULeverAnimInstance* Anim;*/



	FRotator CurrentRotation;
	FRotator TargetRotation;
	float Speed;
	FRotator NewRotation;


	UPROPERTY(EditAnywhere)
	class AKHH_Enemy* me;
		
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LeverMove = false;
};
