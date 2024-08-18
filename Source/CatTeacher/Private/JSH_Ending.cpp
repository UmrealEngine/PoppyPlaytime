// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_Ending.h"

#include "JSH_Cat.h"
#include "JSH_CatDoor.h"
#include "JSH_CatFSM.h"
#include "JSH_EndingCat.h"
#include "KHH_RotateDoor.h"
#include "KMK_PlayerHand.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

class AKMK_PlayerHand;
// Sets default values
AJSH_Ending::AJSH_Ending()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FalseBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FalseBox"));
	FalseBox->SetupAttachment(RootComponent);
	// FalseBox->SetCollisionProfileName(TEXT("NoCollision"));
	FalseBox->SetCollisionProfileName(TEXT("OverlapAll"));
	
	// FalseBox->SetWorldScale3D(FVector(7.0f, 7.0f, 7.0f));
	// FalseBox->SetRelativeLocation(FVector(80.0f, 0.0f, 170.0f));
}

// Called when the game starts or when spawned
void AJSH_Ending::BeginPlay()
{
	Super::BeginPlay();


	// CatDoor 액터 가져오기
	CatDoor = Cast<AJSH_CatDoor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_CatDoor::StaticClass()));

	if (CatDoor)
	{
		// 'Door'라는 이름으로 액터의 참조를 저장합니다.
		FName DoorName = "Door";
		CatDoor->SetActorLabel(DoorName.ToString());
	}

	me = Cast<AJSH_EndingCat>(GetOwner());


	// Retrieve all AJSH_Cat actors in the world
	TArray<AActor*> FoundCats;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AJSH_Cat::StaticClass(), FoundCats);

	// Iterate through the found actors and assign them based on their tags
	for (AActor* Actor : FoundCats)
	{
		AJSH_Cat* FoundCat = Cast<AJSH_Cat>(Actor);
		if (FoundCat)
		{
			if (FoundCat->ActorHasTag("S1"))
			{
				cat1 = FoundCat;
			}
			else if (FoundCat->ActorHasTag("S2"))
			{
				cat2 = FoundCat;
			}
			else if (FoundCat->ActorHasTag("S3"))
			{
				cat3 = FoundCat;
			}
			else if (FoundCat->ActorHasTag("S4"))
			{
				cat4 = FoundCat;
			}
			else if (FoundCat->ActorHasTag("FCatAttack"))
			{
				cat5 = FoundCat;
			}
		}
	}


	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("S5"), FoundActors);

	// 찾은 액터들 중에서 AJSH_Cat 타입의 액터를 CatS5에 저장합니다.
	for (AActor* Actor : FoundActors)
	{
		AJSH_Cat* FoundCat = Cast<AJSH_Cat>(Actor);
		if (FoundCat)
		{
			CatS5 = FoundCat;
			break;  // 첫 번째로 찾은 AJSH_Cat을 저장한 후 루프 종료
		}
	}
	
}

// Called every frame
void AJSH_Ending::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EndingStart)
	{
		CatDoor->fsm->cnt++;
		CatDoor->fsm->isOpen = true;
		static USoundWave* OpenSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Project/JSH/Audio/TopOpen.TopOpen'"));
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
		
		if (cat1 && cat1->ActorHasTag("S1"))
		{
			cat1->fsm->cState = ECatState::ending;
			cat1->fsm->ending = true;
		}
		if (cat2 && cat2->ActorHasTag("S2"))
		{
			cat2->fsm->cState = ECatState::ending;
			cat2->fsm->ending = true;

		}
		if (cat3 && cat3->ActorHasTag("S3"))
		{
			cat3->fsm->cState = ECatState::ending;
			cat3->fsm->ending = true;

		}
		if (cat4 && cat4->ActorHasTag("S4"))
		{
			cat4->fsm->cState = ECatState::ending;
			cat4->fsm->ending = true;

		}
		if (cat5 && cat5->ActorHasTag("FCatAttack"))
		{
			cat5->Destroy();
		}
		
		CatS5->Destroy();
		
		EndingStart = false;
	}


	if (JumpScareEnding)
	{
		if (cat1 && cat1->ActorHasTag("S1"))
		{
			cat1->fsm->cState = ECatState::ending;
			cat1->fsm->ending = true;
		}
		if (cat2 && cat2->ActorHasTag("S2"))
		{
			cat2->fsm->cState = ECatState::ending;
			cat2->fsm->ending = true;

		}
		if (cat3 && cat3->ActorHasTag("S3"))
		{
			cat3->fsm->cState = ECatState::ending;
			cat3->fsm->ending = true;

		}
		if (cat4 && cat4->ActorHasTag("S4"))
		{
			cat4->fsm->cState = ECatState::ending;
			cat4->fsm->ending = true;

		}
		CatS5->Destroy();
		
		JumpScareEnding = false;
	}
}

void AJSH_Ending::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);


	// AKMK_PlayerHand* Hand = Cast<AKMK_PlayerHand>(OtherActor);
	//
	// if (Hand && OtherActor->ActorHasTag("Green"))
	// {
	// 	CatDoor->fsm->cnt++;
	// 	CatDoor->fsm->isOpen = true;
	// 	
	// 	if (cat1 && cat1->ActorHasTag("S1"))
	// 	{
	// 		cat1->fsm->cState = ECatState::ending;
	// 		cat1->fsm->ending = true;
	// 	}
	// 	if (cat2 && cat2->ActorHasTag("S2"))
	// 	{
	// 		cat2->fsm->cState = ECatState::ending;
	// 		cat2->fsm->ending = true;
	//
	// 	}
	// 	if (cat3 && cat3->ActorHasTag("S3"))
	// 	{
	// 		cat3->fsm->cState = ECatState::ending;
	// 		cat3->fsm->ending = true;
	//
	// 	}
	// 	if (cat4 && cat4->ActorHasTag("S4"))
	// 	{
	// 		cat4->fsm->cState = ECatState::ending;
	// 		cat4->fsm->ending = true;
	//
	// 	}
	// 	if (cat5 && cat5->ActorHasTag("S5"))
	// 	{
	// 		cat5->Destroy();
	// 	}
	// 	// Cat의 애니메이션 실행 되도록
	// }
}

