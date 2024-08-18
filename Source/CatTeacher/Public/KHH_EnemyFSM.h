// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "KHH_EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	MoveStop,
	Destroy,
	Spawn,
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UKHH_EnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKHH_EnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	UPROPERTY(EditAnywhere)
	bool isChange = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	EEnemyState mState = EEnemyState::Idle;

	void IdleState();
	void MoveState();
	void MoveStopState();
	void DestroyState();
	void SpawnState();
	void DieState();

	//UFUNCTION()
    //void OnDestroyed(AActor* DestroyedActor);

	UPROPERTY(EditAnywhere, Category=FSM)
	float idleDelayTime = 2;

	float currentTime = 0;

	UPROPERTY(EditAnywhere, Category=FSM)
	class APawn* target;

	UPROPERTY(EditAnywhere)
	class AKHH_Enemy* me;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AAIController* ai;


	FVector destination;
	FVector dir;

	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float P_Speed;
	float Speed;

	UPROPERTY(EditAnywhere)
	class ULeverAnimInstance* Anim;

	//void OnMyStartMove();
	///void OnMyRunStart();

	//int32 count = 0;


	class UKHH_EnemyFSM* destroycomponent;
	class UKHH_EnemyFSM* destroycomponent1;

    UPROPERTY(EditAnywhere)
	TSubclassOf<class AKHH_Enemy> del;

	//UPROPERTY(EditAnywhere)
	//class ULeverComponent* lever;

 //  UPROPERTY(EditAnywhere)
	//class AActor* Player;

	
    UPROPERTY(EditAnywhere)
	class AKMK_PlayerHand* Hand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSMComponent)
	bool isRun = false;


};
