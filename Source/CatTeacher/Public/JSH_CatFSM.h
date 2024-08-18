// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KMK_Player.h"
#include "Components/ActorComponent.h"
#include "JSH_CatFSM.generated.h"


UENUM(BlueprintType)
enum class ECatState : uint8
{
	Idle,
	RoundMove,
	MoveWait,
	TrueMove,
	FalseMoveWait,
	FalseMove,
	Ceiling,
	Attack,
	Discovery,
	top,
	Die,
	ending,
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UJSH_CatFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJSH_CatFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	ECatState cState = ECatState::RoundMove;


	

	//-----------------------------------------------------------------

	// 인터렉션 후 이동 대기 상태
	void IdleState(float DeltaTime);
	
	// 대기 왕복 움직임 상태
	void RoundMoveState();
	
	// 진짜 이동 준비
	void MoveWaitState();

	// 진짜 이동
	void TrueMoveState();

	// 가짜 이동 준비
	void FalseMoveWaitState();

	// 가짜 이동
	void FalseMoveState();

	// 천장
	void CeilingState();

	// 가짜 발견 상태
	void DiscoveryState();
	
	// 공격 상태
	void AttackState();

	// 길 막혔을때
	void TopState();

	// 죽음 상태
	void DieState();

	void EndingState();
	//-----------------------------------------------------------------



	
	
	// 왕복 타깃
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class AJSH_Target* target01;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class AJSH_Target* target02;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class AJSH_Target* targetMiddle;
	
	// player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class AJSH_Target* targetPlayer;


	
	
	

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	//class AJSH_Cat*  AttackCat;

	
	// 소유 액터
	UPROPERTY()
	class AJSH_Cat* me;
	
	

	UPROPERTY(EditAnywhere, Category=FSM)
	float ReachDistance = 150.0f;
	
	UPROPERTY(EditAnywhere, Category=FSM)
	float stTime = 5;

	UPROPERTY(EditAnywhere, Category=FSM)
	float currentTime = 0;

public:
	FTimerHandle TagSelectionTimerHandle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> ccamera;
	
	//------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FSM)
	FName SelectedTag;

	// 상태 변경 함수
	void UpdateState();
	//------------


	//------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FSM)
	FName SelectedTagFalse;
	
	void UpdateStateFalse();
	//------------

	
	bool bHasAttacked = false;

	//bool FalseDie = false;

	
	// idle 상태 관련 
	bool IdleTeleport = false;
	
	UPROPERTY(EditAnywhere, Category = "Control")
	float idletime = 15;
	
	float idlecurrtime = 0;


	// Switch 연기 -> idle 상태 관련
	bool SwSt1 = false;
	bool SwSt2 = false;
	bool SwSt3 = false;
	bool SwSt4 = false;

	// 이동 속도 관련
	UPROPERTY(EditAnywhere, Category = "Control")
	float RoundMoveSpeed = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Control")
	float AttackMoveSpeed = 50.0f;


	// 03, 04 배터리 관련
	UPROPERTY()
	class UKMK_Bat* BatComponent;

	// 처음 03, 04 배터리 일떄 , 03, 04 고양이 없는 상태
	bool FirstBatTrue03 = true;
	bool FirstBatTrue04 = true;


	//
	void BatCatStop();

	void batoff();

	FVector tt;

	// 천장 문
	UPROPERTY()
	class AJSH_CatDoor* CatDoor;
	
	bool DoorOpen = false;
	bool topStart = false;
	
	bool ending = false;


	void End();


	// 공격할때 다른 공격 막기
	// bool AttackCatDestroy = true;

	UPROPERTY()
	class AJSH_Ending* EndingHelper;
};




