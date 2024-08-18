// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h"
#include "KHH_Enemy.generated.h"

UCLASS()
class CATTEACHER_API AKHH_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKHH_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=FSMComponent)
	class UKHH_EnemyFSM* fsm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSMComponent)
	bool isRun = false;

};
