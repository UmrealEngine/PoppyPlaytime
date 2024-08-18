// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_CatDoor.h"

#include "KHH_RotateDoor.h"

// Sets default values
AJSH_CatDoor::AJSH_CatDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	fsm = CreateDefaultSubobject<UKHH_RotateDoor>(TEXT("FSM"));
}

// Called when the game starts or when spawned
void AJSH_CatDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJSH_CatDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

