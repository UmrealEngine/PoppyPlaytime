// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KHH_BossOpendoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UKHH_BossOpendoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKHH_BossOpendoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	class AKMK_Player* Player;

	UPROPERTY(EditAnywhere)
	FVector MoveOffset = FVector(300, 0, 0);
	FVector OriginalLocation;

	UPROPERTY(EditAnywhere)
	float MoveTime = 10;

	UPROPERTY(EditAnywhere)
	bool ShouldMove = false;

	UPROPERTY(EditAnywhere)
	FVector CurrentLocation;
	FVector TargetLocation;
	float Speed;
	FVector NewLocation;


	UPROPERTY(EditAnywhere)
	class USoundCue* BossDoorSound;

	bool BOD = false;

};
