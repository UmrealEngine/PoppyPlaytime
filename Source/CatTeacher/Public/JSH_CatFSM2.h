// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JSH_CatFSM2.generated.h"



UENUM(BlueprintType)
enum class ECatState2 : uint8
{
	RoundMove,
	MoveWait,
	TrueMove,
	FalseMove,
	Ceiling,
	Attack,
	Discovery,
	Blocked,
	Die,
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UJSH_CatFSM2 : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJSH_CatFSM2();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	ECatState2 cState2 = ECatState2::RoundMove;


	

	//-----------------------------------------------------------------

	// 대기 왕복 움직임 상태
	void RoundMoveState();
	
	// 진짜 이동
	void MoveWaitState();

	// 진짜 이동
	void TrueMoveState();

	// 가짜 이동
	void FalseMoveState();

	// 천장
	void CeilingState();

	// 가짜 발견 상태
	void DiscoveryState();
	
	// 공격 상태
	void AttackState();

	// 길 막혔을때
	void BlockedState();

	// 죽음 상태
	void DieState();
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
	class AJSH_Cat2* me;
	
	

	UPROPERTY(EditAnywhere, Category=FSM)
	float ReachDistance = 150.0f;

	// 피격 알림 이벤트 함수
	void OnDamgaeProcess();

	//
	UPROPERTY(EditAnywhere, Category=FSM)
	float stTime = 6;

	UPROPERTY(EditAnywhere, Category=FSM)
	float currentTime = 0;
	
	
};

