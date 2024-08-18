// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_EndingCat.h"

// Sets default values
AJSH_EndingCat::AJSH_EndingCat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJSH_EndingCat::BeginPlay()
{
	Super::BeginPlay();


	soundoff = false;
	
}

// Called every frame
void AJSH_EndingCat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

