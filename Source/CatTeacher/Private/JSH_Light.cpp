// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_Light.h"

// Sets default values
AJSH_Light::AJSH_Light()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AJSH_Light::BeginPlay()
{
	Super::BeginPlay();

	RootComponent->SetVisibility(false);
}

// Called every frame
void AJSH_Light::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LightOn)
	{
		// +++ 사운드 추가
		RootComponent->SetVisibility(true);
	}
	
	if (LightOff)
	{
		// +++ 사운드 추가
		RootComponent->SetVisibility(false);
	}

}

