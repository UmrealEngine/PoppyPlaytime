// Fill out your copyright notice in the Description page of Project Settings.
#include "KHH_OpenDoor.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "KHH_Enemy.h"
#include "KHH_BatteryOpenDoor.h"

// Sets default values for this component's properties
UKHH_OpenDoor::UKHH_OpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKHH_OpenDoor::BeginPlay()
{
	Super::BeginPlay();
	OriginalLocation = GetOwner()->GetActorLocation();

}


// Called every frame
void UKHH_OpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UKHH_BatteryOpenDoor* d = Door->FindComponentByClass<UKHH_BatteryOpenDoor>();
	if (d->enemy != nullptr)
	{
		Player = d->enemy;
	}
	if(Player == nullptr) return;

	
	// 플레이어와 문의 거리 계산
	FVector DistanceVector = Player->GetActorLocation() - GetOwner()->GetActorLocation();
	float Distance = DistanceVector.Length();
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, FString::Printf(TEXT("%f"), Distance));
	// 문을 열어야 할 때
	if (Distance <= DistanceThreshold && !ShouldMove )
	{
		MoveDoor(DeltaTime);
	}
}

void UKHH_OpenDoor::MoveDoor(float DeltaTime)
{
	CurrentLocation = GetOwner()->GetActorLocation();
	TargetLocation = OriginalLocation + MoveOffset;
	Speed = FVector::Distance(OriginalLocation, TargetLocation) / MoveTime;
	NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
	GetOwner()->SetActorLocation(NewLocation);
}


