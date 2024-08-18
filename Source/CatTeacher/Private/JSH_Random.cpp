#include "JSH_Random.h"

#include "EngineUtils.h"
#include "JSH_Cat.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "KMK_Player.h" 
#include "JSH_CatFSM.h" 
#include "JSH_EndingCat.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AJSH_Random::AJSH_Random()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AJSH_Random::BeginPlay()
{
	Super::BeginPlay();

    // 기본 soundplay
    // static USoundWave* SteamSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Project/JSH/Audio/BGM/CatBGM01.CatBGM01'"));
    // UGameplayStatics::PlaySoundAtLocation(GetWorld(), SteamSound, GetActorLocation());
    USoundWave* SteamSound = LoadObject<USoundWave>(nullptr, TEXT("/Game/Project/JSH/Audio/BGM/CatBGM01.CatBGM01"));
    UAudioComponent* AudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), SteamSound);
    BasicBGM = AudioComponent;
    //BasicBGM->Stop();
    
	// Set timer to call FindAndSelectRandomTag every 10 seconds
	//GetWorldTimerManager().SetTimer(TimerHandle, this, &AJSH_Random::FindAndSelectRandomTag, TrueRandomeTime, true);
	// test


    // EndingCat = Cast<AJSH_EndingCat>(GetOwner());
}

// Called every frame
void AJSH_Random::Tick(float DeltaTime)

{
	Super::Tick(DeltaTime);

    if (start)
    {
        ss();
        start = false;
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Start"));
    }

    if (Stop)
    {
        StopRandomTagSelection();
        Stop = false;
    }

    if (EndingSoundOff && BasicBGM != nullptr)
    {
        BasicBGM->Stop();
        DangerBGM->Stop();
    }
}

void AJSH_Random::FindAndSelectRandomTag()
{
    // 필터링할 태그들을 배열로 정의
    TArray<FName> FilteredTags = { FName(TEXT("FCat1")), FName(TEXT("FCat2")), FName(TEXT("FCat3")), FName(TEXT("FCat4")), FName(TEXT("FCat5")) };
    TArray<FName> TagsInLevel;

    // 레벨에 있는 태그 중에서 필터링된 태그들만 찾기
    for (TActorIterator<AJSH_Cat> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        AJSH_Cat* CatActor = *ActorItr;

        for (const FName& Tag : CatActor->Tags)
        {
            if (FilteredTags.Contains(Tag))
            {
                if (!TagsInLevel.Contains(Tag))
                {
                    TagsInLevel.Add(Tag);
                }
            }
        }
    }

    if (TagsInLevel.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, TagsInLevel.Num() - 1);
        FName RandomTag = TagsInLevel[RandomIndex];

        //UE_LOG(LogTemp, Warning, TEXT("Tag: %s"), *RandomTag.ToString());  // 랜덤으로 선택된 태그를 로그로 출력
        
        // FSM 컴포넌트에 랜덤 태그 전달
        for (TActorIterator<AJSH_Cat> ActorItr(GetWorld()); ActorItr; ++ActorItr)
        {
            AJSH_Cat* CatActor = *ActorItr;
            UJSH_CatFSM* CatFSM = CatActor->FindComponentByClass<UJSH_CatFSM>();
            if (CatFSM)
            {
                if (RandomTag == FName(TEXT("FCat5")))
                {
                    // FCat5 태그가 선택되었을 때
                    CatFSM->DoorOpen = true;
                    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Top"));
                }
                if (RandomTag == FName(TEXT("FCat1")))
                {
                    int32 RandomSelection = FMath::RandRange(0, 1);
                    if (CatFSM->me && CatFSM->me->Tags.Contains("FCat1"))
                    {
                        if (RandomSelection == 0)
                        {
                            CatFSM->SelectedTag = RandomTag;
                            RandomSelection = 3;
                            GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Yellow, TEXT("True"));
                        }
                        if (RandomSelection == 1)
                        {
                            CatFSM->SelectedTagFalse = RandomTag;
                            RandomSelection = 3;
                            GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Red, TEXT("False"));
                        }
                    }
                }
                if (RandomTag == FName(TEXT("FCat2")))
                {
                    int32 RandomSelection = FMath::RandRange(0, 1);
                    if (CatFSM->me && CatFSM->me->Tags.Contains("FCat2"))
                    {
                        if (RandomSelection == 0)
                        {
                            CatFSM->SelectedTag = RandomTag;
                            RandomSelection = 3;
                            GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Yellow, TEXT("True"));
                        }
                        if (RandomSelection == 1)
                        {
                            CatFSM->SelectedTagFalse = RandomTag;
                            RandomSelection = 3;
                            GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Red, TEXT("False"));
                        }
                    }
                }
                if (RandomTag == FName(TEXT("FCat3")))
                {
                    int32 RandomSelection = FMath::RandRange(0, 1);
                    if (CatFSM->me && CatFSM->me->Tags.Contains("FCat3"))
                    {
                        if (RandomSelection == 0)
                        {
                            CatFSM->SelectedTag = RandomTag;
                            RandomSelection = 3;
                            GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Yellow, TEXT("True"));
                        }
                        if (RandomSelection == 1)
                        {
                            CatFSM->SelectedTagFalse = RandomTag;
                            RandomSelection = 3;
                            GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Red, TEXT("False"));
                        }
                    }
                }
                if (RandomTag == FName(TEXT("FCat4")))
                {
                    int32 RandomSelection = FMath::RandRange(0, 1);
                    if (CatFSM->me && CatFSM->me->Tags.Contains("FCat4"))
                    {
                        if (RandomSelection == 0)
                        {
                            CatFSM->SelectedTag = RandomTag;
                            RandomSelection = 3;
                            GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Yellow, TEXT("True"));
                        }
                        if (RandomSelection == 1)
                        {
                            CatFSM->SelectedTagFalse = RandomTag;
                            RandomSelection = 3;
                            GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Red, TEXT("False"));
                        }
                    }
                }
                

                
                // else
                // {
                //     // FCat1 ~ FCat4 태그가 선택되었을 때
                //     int32 RandomSelection = FMath::RandRange(0, 1);
                //     if (RandomSelection == 0)
                //     {
                //         CatFSM->SelectedTag = RandomTag;
                //         RandomSelection = 3;
                //         GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Yellow, TEXT("True"));
                //     }
                //     if (RandomSelection == 1)
                //     {
                //         CatFSM->SelectedTagFalse = RandomTag;
                //         RandomSelection = 3;
                //         GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Red, TEXT("False"));
                //     }
                // }
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No True Tags Found in Level"));
    }
}

void AJSH_Random::ss()
{
    
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AJSH_Random::FindAndSelectRandomTag, TrueRandomeTime, true);
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("timer set"));

    // // Danger Sound 재생
    // static USoundWave* SteamSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Project/JSH/Audio/BGM/DangerSound.DangerSound'"));
    // UGameplayStatics::PlaySoundAtLocation(GetWorld(), SteamSound, GetActorLocation());


    USoundWave* DangerSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Project/JSH/Audio/BGM/DangerSound.DangerSound'"));
    UAudioComponent* AudioComponent2 = UGameplayStatics::SpawnSound2D(GetWorld(), DangerSound);
    DangerBGM = AudioComponent2;
    // DangerBGM->Stop();
    
    start = false; // 위에서 막았지만, 한번 더 확실하게 막는
}


void AJSH_Random::StopRandomTagSelection()
{
    // 타이머를 멈추기 위해 ClearTimer 사용
    GetWorldTimerManager().ClearTimer(TimerHandle);
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Timer stopped"));
}