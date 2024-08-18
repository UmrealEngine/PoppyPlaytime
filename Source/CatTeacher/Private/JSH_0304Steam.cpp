// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_0304Steam.h"

#include "JSH_Battery.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AJSH_0304Steam::AJSH_0304Steam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	batfsm = CreateDefaultSubobject<UJSH_Battery>(TEXT("FSM"));

	// Create a Niagara Component
	NSteam = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	SetRootComponent(NSteam);
	
	// Load the Niagara System
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystem(TEXT("/Script/Niagara.NiagaraSystem'/Game/Effect/NG_Steam3.NG_Steam3'"));

	if (NiagaraSystem.Succeeded())
	{
		// Set the Niagara System to the Niagara Component
		NSteam->SetAsset(NiagaraSystem.Object);
	}
	
	NSteam->SetVisibility(true);
}

// Called when the game starts or when spawned
void AJSH_0304Steam::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJSH_0304Steam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 03 ½ºÆÀ ²ô°í Å°±â
	if (ActorHasTag(FName("Bat03Steam")) && batfsm->FSMOnOff03 == false)
	{
		NSteam->SetVisibility(false);
	}
	else if (ActorHasTag(FName("Bat03Steam")) && batfsm->FSMOnOff03 == true)
	{
		NSteam->SetVisibility(true);
	}

	// 04 ½ºÆÀ ²ô°í Å°±â
	if (ActorHasTag(FName("Bat04Steam")) && batfsm->FSMOnOff04 == false)
	{
		NSteam->SetVisibility(false);
	}
	else if (ActorHasTag(FName("Bat04Steam")) && batfsm->FSMOnOff04 == true)
	{
		NSteam->SetVisibility(true);
	}
}

