// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JSH_TopCatFSM.generated.h"


UENUM(BlueprintType)
enum class TCatState : uint8
{
	Idle,
	Prepare,
	TopOpen,
	Attack,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UJSH_TopCatFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJSH_TopCatFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	TCatState tState = TCatState::Idle;

	// 대기 상태
	void IdleState(float DeltaTime);

	// 준비
	void PrepareState(float DeltaTime);
	
	// 천장 열리는 
	void TopOpenState(float DeltaTime);

	// Attack
	void AttackState();


	// 천장 공격 시작
	bool AttackStart = false;
	
	// 천장 문
	UPROPERTY()
	class AJSH_CatDoor* CatDoor;
	
	bool DoorOpen = false;
	bool topStart = false;

	// Attack 시간 계산

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float currtime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float toptime = 3.0f;


	// attack bool
	bool bHasAttacked = false;
	FVector tt;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> TopCat;


	// Light
	//class AJSH_Light* DoorLight;

	UPROPERTY()
	TArray<class AJSH_Light*> DoorLights;

	
	float lighttime = 0;
	float opentime = 3;


	// class AKMK_PlayerHand* PlayerHand;

	bool start = false;



	UPROPERTY(EditAnywhere)
	class AJSH_Cat* Cat;

	// 태그 부여 시간
	float addtag = 20.0f;


	UPROPERTY(EditAnywhere)
	class AJSH_Cat* CatS5;


	UPROPERTY()
	class AJSH_Ending* EndingHelper;
};
