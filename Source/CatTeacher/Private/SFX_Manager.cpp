// Fill out your copyright notice in the Description page of Project Settings.


#include "SFX_Manager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
ASFX_Manager::ASFX_Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASFX_Manager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASFX_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!BGMmanager) {
		return;
	}
	if (BGMCount <= 0)
	{
		audio = UGameplayStatics::SpawnSound2D(GetWorld(), BGM, 0.45f, 1.0f, 0.f, nullptr, true, false);
		audio->Play();
		BGMCount++;
	}
}

