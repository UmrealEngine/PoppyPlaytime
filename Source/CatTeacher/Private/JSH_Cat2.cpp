// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_Cat2.h"
#include "JSH_CatFSM2.h"

// Sets default values
AJSH_Cat2::AJSH_Cat2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AJSH_Cat2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJSH_Cat2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJSH_Cat2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

