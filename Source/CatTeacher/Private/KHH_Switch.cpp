// Fill out your copyright notice in the Description page of Project Settings.


#include "KHH_Switch.h"
#include "KMK_PlayerHand.h"
#include "GameFramework/Actor.h"
#include "KHH_BossOpendoor.h"
#include "KMK_PlayerHandFSM.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "KHH_BatteryOpenDoor.h"

// Sets default values
AKHH_Switch::AKHH_Switch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AKHH_Switch::BeginPlay()
{
	Super::BeginPlay();
	//BoxComp->OnComponentBeginOverlap.AddDynamic(this,&AKHH_Switch::OnMyBoxBeginOverlap);
}

// Called every frame
void AKHH_Switch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKHH_Switch::Opendoor()
{
	if (door != nullptr)
	{
		BatteryOpendoorComponent = door->FindComponentByClass<UKHH_BatteryOpenDoor>();

		if (BatteryOpendoorComponent)
		{
			BatteryOpendoorComponent->ShouldMove = true;
		}
	}
}

void AKHH_Switch::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	AKMK_PlayerHand* Hand = Cast<AKMK_PlayerHand>(OtherActor);

	if (Hand && OtherActor->ActorHasTag("Green"))
	{
		FSM = Hand->FSM;
		if (FSM && FSM->isCharge == true)
		{	
			FSM->PState = PlayerHandFSM::Normal;
			FSM->isCharge = false;
			if (Hand->overActor->ActorHasTag("Switch1"))
			{
				Opendoor();
			}
			else if (Hand->overActor->ActorHasTag("Switch2"))
			{
				Opendoor();
			}
			else if (Hand->overActor->ActorHasTag("Switch3"))
			{
				lastDoor = true;
			}

		}

	}
}

