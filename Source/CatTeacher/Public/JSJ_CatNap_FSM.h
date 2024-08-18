// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JSJ_CatNap_FSM.generated.h"
// State 변수들
UENUM()
enum class CATNAP_State : uint8
{
	C_Idle,
	C_Move,
	C_Attack,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UJSJ_CatNap_FSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJSJ_CatNap_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	


public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// 스테이트 enum 변수를 쓰기 위한 변수
	UPROPERTY()
	CATNAP_State cState = CATNAP_State::C_Idle;

	void IdleState();
	void MoveState();
	void AttackState();
};
