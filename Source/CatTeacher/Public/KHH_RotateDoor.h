// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KHH_RotateDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UKHH_RotateDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKHH_RotateDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	FRotator OpenAngle;
	FRotator InitialRotation;

	UPROPERTY(EditAnywhere)
	float MoveTime = 0.7;
	UPROPERTY(EditAnywhere)
	float OpenTime = 15;

	UPROPERTY(EditAnywhere)
	float DistanceThreshold = 150.0f;

	UPROPERTY(EditAnywhere)
	bool ShouldMove = false;

	UPROPERTY(EditAnywhere)
	class AKMK_Player* Target;

	FRotator CurrentRotation;
	FRotator TargetRotation;
	float Speed;
	FRotator NewRotation;

	void RotateDoor(float DeltaTime);	
	void RotateDoor1(float DeltaTime, FRotator angle, float time);

	UPROPERTY(EditAnywhere)
	bool isOpen = false;

	bool isComp = false;
	
	bool isRight = false;
	bool isLeft = false;

	int cnt = 0;

	UPROPERTY(EditAnywhere)
	class USoundCue* RotateDoorSound;

	bool RDS = false;
		
};
