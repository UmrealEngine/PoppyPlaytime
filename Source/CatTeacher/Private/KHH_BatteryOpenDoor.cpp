// Fill out your copyright notice in the Description page of Project Settings.


#include "KHH_BatteryOpenDoor.h"
#include "KHH_Switch.h"
#include "KMK_Bat.h"
#include "KHH_Enemy.h"
#include "KHH_EnemyFSM.h"
#include "LeverComponent.h"
#include "LeverAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values for this component's properties
UKHH_BatteryOpenDoor::UKHH_BatteryOpenDoor()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}
// 하고싶은일 : 스위치, 배터리, 배터리 3개가 충족될 때 마지막 셔터가 열려야한다. 
// 해야하는일 : 
// 1. charge 된 상태, IShasPush 연결, 
// 1-1 직접 할당
//  // 필요요소 : 
// 2. 내가 마지막문이라면
// 3. 가져온 변수들이 모두 True일때
// 4. movedoor을 할꺼다

// Called when the game starts
void UKHH_BatteryOpenDoor::BeginPlay()
{
    Super::BeginPlay();
    OriginalLocation = GetOwner()->GetActorLocation();
}

// Called every frame
void UKHH_BatteryOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (GetOwner()->GetActorLabel().Contains("MG6"))
    {
        SwitchComponent = Cast<AKHH_Switch>(doors[0]);
        BatComponent = doors[1]->FindComponentByClass<UKMK_Bat>();
        BatComponent1 = doors[2]->FindComponentByClass<UKMK_Bat>();
        
        if (SwitchComponent->lastDoor == true && BatComponent->isHaveBat == true && BatComponent1->isHaveBat == true)
        {
            MoveDoor(DeltaTime);
            if (!BOD) {
                BOD = true;
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), BatOpenDoor, GetOwner()->GetActorLocation());

            }
        }
    }

    else {
        if (ShouldMove) // 배터리가 들어갔을때 조건을 맞춰 넣어야함 // 
        {   
            MoveDoor(DeltaTime);
            if (!BOD){
                BOD = true;
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), BatOpenDoor, GetOwner()->GetActorLocation());
            }
        }
    }
}

void UKHH_BatteryOpenDoor::MoveDoor(float DeltaTime)
{
    CurrentLocation = GetOwner()->GetActorLocation();
    TargetLocation = OriginalLocation + MoveOffset;
    Speed = FVector::Distance(OriginalLocation, TargetLocation) / MoveTime;
    NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
    GetOwner()->SetActorLocation(NewLocation);
}

void UKHH_BatteryOpenDoor::SpawnEnemy()
{
	destroycomponent = Player->FindComponentByClass<UKHH_EnemyFSM>();
	destroycomponent->mState = EEnemyState::Destroy;

    if (count == 0)
    {   
        // (X = 1100.000000, Y = 290.000000, Z = 68.000000)
        FTransform spawnLocation = FTransform(FVector(1100.000000, 290.000000, 68.000004));


        //(1091.563649, 394.258328, 68.000004) 셔터 앞
        //(2561.733490, 393.345482, 68.000018) lever 앞

        //(221.731950, 416.215528, 67.999995)

        enemy = GetWorld()->SpawnActor<AKHH_Enemy>(del, spawnLocation);

        auto* comp = lever->FindComponentByClass<ULeverComponent>();

        if (comp)
        {
            comp->me = enemy;
        }

        destroycomponent1 = enemy->FindComponentByClass<UKHH_EnemyFSM>();
        destroycomponent1->mState = EEnemyState::Spawn;
        count++;
    }
}