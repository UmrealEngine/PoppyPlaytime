// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_CatFSM2.h"

#include "JSH_Cat2.h"
#include "KMK_Player.h"
#include "Kismet/GameplayStatics.h"
#include "JSH_Target.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UJSH_CatFSM2::UJSH_CatFSM2()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UJSH_CatFSM2::BeginPlay()
{
	Super::BeginPlay();
	
	// 월드에서 Player 찾아오기
	//auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_Cat::StaticClass());
	//layer = Cast<AJSH_Cat>(actor);
	
	// 소유 객체 가져오기
	me = Cast<AJSH_Cat2>(GetOwner());

	cState2 = ECatState2::RoundMove;

	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(me->GetCharacterMovement());
	if (MovementComponent)
	{
		MovementComponent->MaxWalkSpeed = 300.0f;  // 원하는 최대 속도로 설정
	}


}


// Called every frame
void UJSH_CatFSM2::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (cState2)
	{
	case ECatState2::RoundMove:
		RoundMoveState();
		break;
		
	case ECatState2::MoveWait:
		MoveWaitState();
		break;
		
	case ECatState2::TrueMove:
		TrueMoveState();
		break;

	case ECatState2::FalseMove:
		FalseMoveState();
		break;

	case ECatState2::Ceiling:
		CeilingState();
		break;
		
	case ECatState2::Discovery:
		DiscoveryState();
		break;
		
	case ECatState2::Attack:
		AttackState();
		break;

	case ECatState2::Blocked:
		BlockedState();
		break;
		
	case ECatState2::Die:
		DieState();
		break;
	}

	FString logMsg = UEnum::GetValueAsString(cState2);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMsg);
}

void UJSH_CatFSM2::RoundMoveState()
{
    static bool bMovingToTarget01_FCat2 = true; // FCat2 대상에 대한 이동 플래그
	
    TArray<AActor*> TaggedActors_FCat2;

    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat2"), TaggedActors_FCat2);
	
    // me가 FCat2 태그를 가지고 있는지 확인
    bool bIsTagged_FCat2 = TaggedActors_FCat2.Contains(me);

    if (bIsTagged_FCat2)
    {
        // me가 FCat2 태그를 가진 경우에만 아래 코드를 실행
        FVector destination = bMovingToTarget01_FCat2 ? target01->GetActorLocation() : target02->GetActorLocation();

        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        if (FVector::Dist(me->GetActorLocation(), destination) < ReachDistance)
        {
            // 목표 지점 도착, 이동 목표를 전환
            bMovingToTarget01_FCat2 = !bMovingToTarget01_FCat2;
        	//cState2 = ECatState2::MoveWait;
        }
    }
}

void UJSH_CatFSM2::MoveWaitState()
{
	// 태그 삭제
	if (me && me->Tags.Contains(FName("FCat2"))) 
	{
		me->Tags.Remove(FName("FCat2"));
	}
	
	FVector destinationM = targetMiddle->GetActorLocation();
	FVector dirM = destinationM - me->GetActorLocation();
	me->AddMovementInput(dirM.GetSafeNormal());
	if (dirM.Size() < ReachDistance)
	{
		// 상태 전환
		cState2 = ECatState2::TrueMove;
	}
}



void UJSH_CatFSM2::TrueMoveState()
{
	FVector destinationPlayer = targetPlayer->GetActorLocation();
	FVector dirPlayer = destinationPlayer - me->GetActorLocation();
	me->AddMovementInput(dirPlayer.GetSafeNormal());


	if (dirPlayer.Size() < ReachDistance)
	{
		// 상태 전환
		cState2 = ECatState2::Attack;
	}
	// 특정 위치 도달 -> AttackState()  ___ Game Over
	// (환영) 앞으로가다가 , 신호탄 맞음 -> DiscoveryState()  ___ 천천히 사라짐
	// (진짜)  앞으로가다가 player가 기믹 실행 ->  DieState() ___ destroy 후 본래 왕복 위치 spawn 
}


void UJSH_CatFSM2::FalseMoveState()
{

}

void UJSH_CatFSM2::CeilingState()
{
}

void UJSH_CatFSM2::DiscoveryState()
{
}

void UJSH_CatFSM2::AttackState()
{
}

void UJSH_CatFSM2::BlockedState()
{
}

void UJSH_CatFSM2::DieState()
{
}

void UJSH_CatFSM2::OnDamgaeProcess()
{
}