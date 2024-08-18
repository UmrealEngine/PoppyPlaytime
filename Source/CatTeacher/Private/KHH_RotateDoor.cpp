// Fill out your copyright notice in the Description page of Project Settings.
#include "KHH_RotateDoor.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "KMK_Player.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UKHH_RotateDoor::UKHH_RotateDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UKHH_RotateDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialRotation = GetOwner()->GetActorRotation();
}


// Called every frames
void UKHH_RotateDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (isRight || isLeft)
	{
		isOpen = false;
	}
	if (!isRight && !isLeft && cnt > 0)
	{
		isOpen = true;
		if (GetOwner()->GetActorRotation().Pitch > 50)
		{
			isOpen = false;
			isRight = false;
			isLeft = false;
			cnt = 0;
		}
	}
	if (isOpen)
	{
		RotateDoor1(DeltaTime, FRotator(50, 0, 0), OpenTime);
		cnt++;
		if (GetOwner()->GetActorRotation().Pitch > 50)
		{
			isOpen = false;
			isRight = false;
			isLeft = false;
			cnt = 0;
		}
	}

	if (!isOpen && cnt > 0)
	{
        cnt = 0;
	}
	Target = Cast<AKMK_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (!Target || isOpen) return;

	// 플레이어와 문의 거리 계산
	FVector DistanceVector = Target->GetActorLocation() - GetOwner()->GetActorLocation();

	float Distance = DistanceVector.Length();

	// 문을 열어야 할 때
	if (Distance <= DistanceThreshold && !ShouldMove)
	{
		RotateDoor(DeltaTime);
		if (!RDS) {
			RDS = true;
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), RotateDoorSound, GetOwner()->GetActorLocation());
		}
	}
}

void UKHH_RotateDoor::RotateDoor(float DeltaTime)
{
	CurrentRotation = GetOwner()->GetActorRotation();
	TargetRotation = InitialRotation + OpenAngle;
	Speed = FRotator::NormalizeAxis((TargetRotation - InitialRotation).Yaw) / MoveTime;
	if (Speed < 0)
	{
		Speed *= -1;
	}
	NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, Speed);
	GetOwner()->SetActorRotation(NewRotation);
}

void UKHH_RotateDoor::RotateDoor1(float DeltaTime, FRotator angle, float time)
{
	CurrentRotation = GetOwner()->GetActorRotation();
	TargetRotation = InitialRotation + angle;
	Speed = FRotator::NormalizeAxis((TargetRotation - InitialRotation).Pitch) / time;
	if (Speed < 0)
	{
		Speed *= -1;
	}
	NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, Speed);
	GetOwner()->SetActorRotation(NewRotation);
}

