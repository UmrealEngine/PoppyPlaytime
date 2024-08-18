// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KMK_PlayerHand.generated.h"

// 플레이어 장갑을 위한 FSM
UENUM()
enum class HandState
{
	Grab,
	Go,
	Reverse,
	None
};

UCLASS()
class CATTEACHER_API AKMK_PlayerHand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKMK_PlayerHand();
	UPROPERTY(EditAnywhere, Category = "Hand",BlueprintReadWrite)
	class UStaticMeshComponent* hand = nullptr;
	UPROPERTY(EditAnywhere, Category = "Hand")
	TArray<class UStaticMesh*> HandMesh;
	UPROPERTY(EditAnywhere, Category = "Hand")
	class UArrowComponent* arrow;
	UPROPERTY(EditAnywhere, Category = "Hand")
	class UBoxComponent* box;
	UPROPERTY()
	class AKMK_Player* player;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	HandState PState = HandState::None;
	int n = 0;
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* VFXComp;
	UPROPERTY()
	bool isGo = false;
	bool isReverse = false;
	bool isRay = false;
	float handPos;
	FVector pos;
	FVector startPos;
	FVector endPos;
	FVector dir;
	
	UPROPERTY(EditAnywhere, Category = "Move")
	float speed = 100;

	float t = 0;
	UPROPERTY(EditAnywhere, Category = "Shoot")
	float ShootTime = 3;
	
	UPROPERTY(EditAnywhere)
	class UKMK_PlayerHandFSM* FSM;
	// 물건을 잡을 수 있게하는 컴포넌트
	UPROPERTY()
	class UPhysicsHandleComponent* handle = nullptr;
	UPROPERTY()
	class UPrimitiveComponent* hitinfo;
	UPROPERTY(EditAnywhere)
	bool isGrab = false;

	float time = 0;
	
	bool isRight = false;
	bool isLeft = false;

	UFUNCTION()
	// 오버랩 감지를 위한 함수 선언
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);

	UPROPERTY()
	class AKMK_Battery* grabActor;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AKMK_Battery> BatteryFact;
	FTransform trans;

	FVector pickTrans;
	bool isPick = false;

	bool isCome = false;

	bool isJump = false;

	UPROPERTY()
	bool isBatCom = false;

	UPROPERTY()
	class UKMK_Bat* b;

	FString SwitchName;

	bool isHold = false;
	float holdTime = 0;


	UPROPERTY(EditAnywhere)
	float HandTime = 0.5f;

	bool isGet = false;
	UPROPERTY()
	class UKHH_RotateDoor* rotDoor;
	bool isDoor = false;
	FRotator firstRot;
	FRotator rot;

	UPROPERTY(EditAnywhere)
	bool isClosed = false;
	UPROPERTY(EditAnywhere)
	bool isDie = false;
	UFUNCTION()
	void MakeBattery(int32 num);

	UPROPERTY(EditAnywhere)
    class UMaterialInstance* matFact;

	UPROPERTY()
	UMaterialInstanceDynamic* myMatDynamic;

	UPROPERTY()
	class ULeverComponent* lever;
	UPROPERTY(EditAnywhere)
	bool isLever = false;
	FRotator leverRot;

	// BeginOverlap된 액터를 저장하는 변수
	UPROPERTY()
	AActor* overActor;

	// Ending을 위한 
	UPROPERTY()
	class AJSH_EndingCat* EndingCat;

	UPROPERTY()
	class AJSH_Ending* EndingHelper;

	int gCount = 0;
	int rCount = 0;
};
