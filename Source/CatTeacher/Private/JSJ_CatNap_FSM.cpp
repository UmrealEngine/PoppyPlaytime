// Fill out your copyright notice in the Description page of Project Settings.


#include "JSJ_CatNap_FSM.h"

// Sets default values for this component's properties
UJSJ_CatNap_FSM::UJSJ_CatNap_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJSJ_CatNap_FSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UJSJ_CatNap_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// FSM 변경 함수

	// m_state에 따른 변화
	switch (cState)
	{
	case CATNAP_State::C_Idle:
		IdleState();
		break;
	case CATNAP_State::C_Move:
		MoveState();
		break;

	case CATNAP_State::C_Attack:
		AttackState();
		break;
	}
}


#pragma region IdleState
void  UJSJ_CatNap_FSM::IdleState()
{

}
#pragma endregion


#pragma region MoveState
void  UJSJ_CatNap_FSM::MoveState()
{

}
#pragma endregion

#pragma region AttackState
void  UJSJ_CatNap_FSM::AttackState()
{

}
#pragma endregion

