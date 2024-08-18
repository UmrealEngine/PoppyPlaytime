// Fill out your copyright notice in the Description page of Project Settings.


#include "KHH_Enemy.h"
#include "KHH_EnemyFSM.h"


// Sets default values
AKHH_Enemy::AKHH_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));

	if (tempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(tempMesh.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}
	fsm = CreateDefaultSubobject<UKHH_EnemyFSM>(TEXT("FSM"));

}

// Called when the game starts or when spawned
void AKHH_Enemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AKHH_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isRun == true)
	{
		fsm->isRun = true;
	}
}

// Called to bind functionality to input
void AKHH_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



