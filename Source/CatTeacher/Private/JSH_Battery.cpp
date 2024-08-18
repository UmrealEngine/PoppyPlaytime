// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_Battery.h"
#include "KMK_Bat.h"
#include "JSH_0304Steam.h"
#include "JSH_CatFSM.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UJSH_Battery::UJSH_Battery()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJSH_Battery::BeginPlay()
{
	Super::BeginPlay();

	
	FindTaggedActors();

	Cat03Component = Cat03->FindComponentByClass<UJSH_CatFSM>();
	Cat04Component = Cat04->FindComponentByClass<UJSH_CatFSM>();

	BatComponent03 = batsave03->FindComponentByClass<UKMK_Bat>();
	FSMOnOff03 = BatComponent03->isHaveBat;

	BatComponent04 = batsave04->FindComponentByClass<UKMK_Bat>();
	FSMOnOff04 = BatComponent04->isHaveBat;
}


// Called every frame
void UJSH_Battery::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	// 민경 배터리 isHaveBat == true 됬을떄
	// niagara 꺼지고 , 3번 문 fsm 활성화


	if (BatComponent03->isHaveBat == false)
	{
		if (repeat03)
		{
			FSMOnOff03 = false;
			Cat03Component->batoff();
			repeat03 = false;
		}
	}
	if (BatComponent03->isHaveBat == true)
	{
		FSMOnOff03 = true;
		Cat03Component->BatCatStop();
		repeat03 = true;
	}


	if (BatComponent04->isHaveBat == false)
	{
		if (repeat04)
		{
			FSMOnOff04 = false;
			Cat04Component->batoff();
			repeat04 = false;
		}
	}
	
	if (BatComponent04->isHaveBat == true)
	{
		FSMOnOff04 = true;
		Cat04Component->BatCatStop();
		repeat04 = true;
	}
}

void UJSH_Battery::FindTaggedActors()
{
	UWorld* World = GetWorld();
	if (World)
	{
		// Arrays to store found actors
		TArray<AActor*> FoundActors03;
		TArray<AActor*> FoundActors04;

		// Find actors with tag "bat03"
		UGameplayStatics::GetAllActorsWithTag(World, FName("bat03"), FoundActors03);
		if (FoundActors03.Num() > 0)
		{
			batsave03 = FoundActors03[0];
		}

		// Find actors with tag "bat04"
		UGameplayStatics::GetAllActorsWithTag(World, FName("bat04"), FoundActors04);
		if (FoundActors04.Num() > 0)
		{
			batsave04 = FoundActors04[0];
		}



		
		//  FSM 가져올 고양이 찾기
		TArray<AActor*> FCat03;
		
		UGameplayStatics::GetAllActorsWithTag(World, FName("S3"), FCat03);
		if (FCat03.Num() > 0)
		{
			Cat03 = FCat03[0];
		}

		TArray<AActor*> FCat04;
		
		UGameplayStatics::GetAllActorsWithTag(World, FName("S4"), FCat04);
		if (FCat04.Num() > 0)
		{
			Cat04 = FCat04[0];
		}
		

	}
}

