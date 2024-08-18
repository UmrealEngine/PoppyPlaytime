// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_Steam.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AJSH_Steam::AJSH_Steam()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create a Niagara Component
	NSteam = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	SetRootComponent(NSteam);
	
	// Load the Niagara System
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystem(TEXT("/Script/Niagara.NiagaraSystem'/Game/Effect/NG_Steam2.NG_Steam2'"));

	if (NiagaraSystem.Succeeded())
	{
		// Set the Niagara System to the Niagara Component
		NSteam->SetAsset(NiagaraSystem.Object);
	}
	
	NSteam->SetVisibility(false);
}

// Called when the game starts or when spawned
void AJSH_Steam::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJSH_Steam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SteamON)
	{
		NSteam->SetVisibility(true);
		currtime += DeltaTime;

		if (soundstart)
		{
			static USoundWave* SteamSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Project/JSH/Audio/CatSteam2.CatSteam2'"));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SteamSound, GetActorLocation());
			soundstart = false;
		}
		
		if (currtime >= offtime)
		{
			NSteam->SetVisibility(false);
			SteamON = false;
			currtime = 0;
		}
	}
}

