	// Fill out your copyright notice in the Description page of Project Settings.


	#include "JSH_Switch.h"

	#include "JSH_Cat.h"
	#include "JSH_CatFSM.h"
	#include "JSH_Steam.h"
	#include "KMK_PlayerHand.h"
	#include "KMK_PlayerHandFSM.h"
	#include "Kismet/GameplayStatics.h"

	// Sets default values
	AJSH_Switch::AJSH_Switch()
	{
 		// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
		PrimaryActorTick.bCanEverTick = true;

	}

	// Called when the game starts or when spawned

	void AJSH_Switch::BeginPlay()
	{
		Super::BeginPlay();
    
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AJSH_Steam::StaticClass(), FoundActors);

		if (FoundActors.Num() > 0)
		{
			if (FoundActors.IsValidIndex(0))
			{
				Steam01 = Cast<AJSH_Steam>(FoundActors[0]);
			}
			if (FoundActors.IsValidIndex(1))
			{
				Steam02 = Cast<AJSH_Steam>(FoundActors[1]);
			}
			if (FoundActors.IsValidIndex(2))
			{
				Steam03 = Cast<AJSH_Steam>(FoundActors[2]);
			}
			if (FoundActors.IsValidIndex(3))
			{
				Steam04 = Cast<AJSH_Steam>(FoundActors[3]);
			}
		}

		
	}

	// Called every frame
	void AJSH_Switch::Tick(float DeltaTime)
	{
		Super::Tick(DeltaTime);
		
	}



void AJSH_Switch::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    AKMK_PlayerHand* Hand = Cast<AKMK_PlayerHand>(OtherActor);
    //AJSH_Cat* Cat = Cast<AJSH_Cat>(OtherActor);

    if (Hand && OtherActor->ActorHasTag("Green"))
    {
        FSM = Hand->FSM;
    	

        if (FSM && FSM->isCharge == true)
        {
            GEngine->AddOnScreenDebugMessage(8, 3, FColor::Yellow, FString::Printf(TEXT("On Switch")));
            FSM->isCharge = false;
        	FSM->PState = PlayerHandFSM::Normal;

            if (Hand->overActor->ActorHasTag(FName("Switch1")))
            {
                // 1번 통로 연기 -> cat01 destroy
                GEngine->AddOnScreenDebugMessage(31, 3, FColor::Red, FString::Printf(TEXT("C1")));

                TArray<AActor*> FoundActors;
                UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("S1"), FoundActors);

                // 찾은 액터들 순회
                for (AActor* Actor : FoundActors)
                {
                    AJSH_Cat* Cat = Cast<AJSH_Cat>(Actor);
                    if (Cat)
                    {
                        // tt 변수를 true로 설정
                        Cat->fsm->SwSt1 = true;
                        Cat->Tags.Remove("FCat1");
                    }
                }
            	// static USoundWave* SteamSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Project/JSH/Audio/CatSteam2.CatSteam2'"));
            	// UGameplayStatics::PlaySoundAtLocation(GetWorld(), SteamSound, GetActorLocation());
            	Steam01->SteamON = true;
            	Steam01->soundstart = true;
            }
            else if (Hand->overActor->ActorHasTag(FName("Switch2")))
            {
                // 2번 통로 연기 -> cat01 destroy
                GEngine->AddOnScreenDebugMessage(31, 3, FColor::Yellow, FString::Printf(TEXT("C2")));

            	TArray<AActor*> FoundActors;
            	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("S2"), FoundActors);

            	// 찾은 액터들 순회
            	for (AActor* Actor : FoundActors)
            	{
            		AJSH_Cat* Cat = Cast<AJSH_Cat>(Actor);
            		if (Cat)
            		{
            			// tt 변수를 true로 설정
            			Cat->fsm->SwSt2 = true;
            			Cat->Tags.Remove("2");
            		}
            	}

                Steam02->SteamON = true; // Only set Steam02 to true for BP_Switch_C_0
            	Steam02->soundstart = true;
            }
            // 3번 통로
            else if (Hand->overActor->ActorHasTag(FName("Switch3")))
            {
                // 3번 통로 연기 -> cat01 destroy
                GEngine->AddOnScreenDebugMessage(31, 3, FColor::Yellow, FString::Printf(TEXT("C3")));
            	
            	TArray<AActor*> FoundActors;
            	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("S3"), FoundActors);

            	// 찾은 액터들 순회
            	for (AActor* Actor : FoundActors)
            	{
            		AJSH_Cat* Cat = Cast<AJSH_Cat>(Actor);
            		if (Cat)
            		{
            			// tt 변수를 true로 설정
            			Cat->fsm->SwSt3 = true;
            			Cat->Tags.Remove("FCat3");
            		}
            	}

                Steam03->SteamON = true; // Only set Steam03 to true for BP_Switch_C_2
            	Steam03->soundstart = true;
            }
            // 4번 통로
            else if (Hand->overActor->ActorHasTag(FName("Switch4")))
            {
                // 4번 통로 연기 -> cat01 destroy
                GEngine->AddOnScreenDebugMessage(31, 3, FColor::Yellow, FString::Printf(TEXT("C4")));
            	
            	TArray<AActor*> FoundActors;
            	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("S4"), FoundActors);



            	// 찾은 액터들 순회
            	for (AActor* Actor : FoundActors)
            	{
            		AJSH_Cat* Cat = Cast<AJSH_Cat>(Actor);
            		if (Cat)
            		{
            			// tt 변수를 true로 설정
            			Cat->fsm->SwSt4 = true;
            			Cat->Tags.Remove("FCat4");
            		}
            	}
                Steam04->SteamON = true; // Only set Steam04 to true for BP_Switch_C_3
            	Steam04->soundstart = true;
            }
        }
    }
}


