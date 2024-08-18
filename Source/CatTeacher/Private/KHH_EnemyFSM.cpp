// Fill out your copyright notice in the Description page of Project Settings.


#include "KHH_EnemyFSM.h"
#include <AIController.h>
#include <NavigationSystem.h>
#include "KHH_Enemy.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "LeverAnimInstance.h"
#include "LeverComponent.h"
#include "GameFramework/Actor.h"
#include "KMK_PlayerHand.h"
#include "Sound/SoundCue.h"


// Sets default values for this component's properties
UKHH_EnemyFSM::UKHH_EnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//lever = Player->FindComponentByClass<ULeverComponent>();
	

	// ...
}


// Called when the game starts
void UKHH_EnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<AKHH_Enemy>(GetOwner());
	ai = Cast<AAIController>(me->GetController());
	target = GetWorld()->GetFirstPlayerController()->GetPawn();
	//AAIController
	Speed = P_Speed;

	/*if (me)
	{
		me->OnDestroyed.AddDynamic(this, &UKHH_EnemyFSM::OnDestroyed);
	}*/

	Anim = Cast<ULeverAnimInstance>(me->GetMesh()->GetAnimInstance());
	Hand = Cast<AKMK_PlayerHand>(UGameplayStatics::GetActorOfClass(GetWorld(),AKMK_PlayerHand::StaticClass()));
}


// Called every frame
void UKHH_EnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...

	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(1, 1, FColor::Cyan, logMsg);

	switch (mState) {

	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;

	case EEnemyState::MoveStop:
		MoveStopState();
		break;

	case EEnemyState::Destroy:
		DestroyState();
		break;
	
	case EEnemyState::Spawn:
		SpawnState();
		break;	

	case EEnemyState::Die:
		DieState();
		break;
	}
}


void UKHH_EnemyFSM::IdleState()
{
	currentTime += GetWorld()->GetDeltaSeconds();
	if (currentTime > idleDelayTime)
	{
		mState = EEnemyState::Move;
		Anim->DelightState = mState;
		currentTime = 0;
	}
}


void UKHH_EnemyFSM::MoveState()
{
	
	P_Speed = Speed;
	destination = target->GetTargetLocation();
	dir = destination - me->GetActorLocation();
	dir.Normalize();
	ai->MoveToLocation(destination);
	
}	

void UKHH_EnemyFSM::MoveStopState()
{
	//player랑 눈이 마주치면 멈춰야함 
	//눈이 안마주치면 그대로 move, 따라가야함 
	// player의 speed를 0으로 
	P_Speed = 0;
}

void UKHH_EnemyFSM::DestroyState()
{	
	if (me)
	{
		me->SetActorHiddenInGame(true);
		me->SetActorEnableCollision(false);
		me->SetActorTickEnabled(false);
	}
}

void UKHH_EnemyFSM::SpawnState()
{
	
	Anim->DelightState=mState;
	GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 200 * GetWorld()->GetDeltaSeconds());

	float distance = FVector::Distance(target->GetActorLocation(), GetOwner()->GetActorLocation());

	if (isRun == true)
	{
		
		mState = EEnemyState::Die;
		Anim->DelightState = mState;
	}
	
}

void UKHH_EnemyFSM::DieState()
{
	// 달려오는 애니메이션으로 전환

	GetOwner()->SetActorLocation(FVector(2600.989926, 290,69.763294));
	//(2582.989926, 394.258331,69.763294)
}



