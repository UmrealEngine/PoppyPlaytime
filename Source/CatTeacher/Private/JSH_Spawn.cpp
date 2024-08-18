#include "JSH_Spawn.h"
#include "EngineUtils.h"
#include "JSH_Cat.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

// Sets default values
AJSH_Spawn::AJSH_Spawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AJSH_Spawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AJSH_Spawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 태그 확인 함수 호출
	CheckTagsAndLog();
}

void AJSH_Spawn::CheckTagsAndLog()
{
	bool bFcat1Exists = false;
	bool bFcat2Exists = false;
	bool bFcat3Exists = false;
	bool bFcat4Exists = false;

	// 맵에 있는 AJSH_Cat의 태그를 확인
	for (TActorIterator<AJSH_Cat> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AJSH_Cat* CatActor = *ActorItr;
		if (CatActor->Tags.Contains(FName("S1")))
		{
			bFcat1Exists = true;
		}
		if (CatActor->Tags.Contains(FName("S2")))
		{
			bFcat2Exists = true;
		}
		if (CatActor->Tags.Contains(FName("S3")))
		{
			bFcat3Exists = true;
		}
		if (CatActor->Tags.Contains(FName("S4")))
		{
			bFcat4Exists = true;
		}
	}

	// 태그가 없는 경우 5초 후 화면에 메시지 표시
	if (!bFcat1Exists && !bCheckingFcat1)
	{
		bCheckingFcat1 = true;
		GetWorldTimerManager().SetTimer(TimerHandle1, [this]() { DisplayMissingTag(FName("S1")); bCheckingFcat1 = false; }, 0.5f, false);
	}
	if (!bFcat2Exists && !bCheckingFcat2)
	{
		bCheckingFcat2 = true;
		GetWorldTimerManager().SetTimer(TimerHandle2, [this]() { DisplayMissingTag(FName("S2")); bCheckingFcat2 = false; }, 0.5f, false);
	}
	if (!bFcat3Exists && !bCheckingFcat3)
	{
		bCheckingFcat3 = true;
		GetWorldTimerManager().SetTimer(TimerHandle3, [this]() { DisplayMissingTag(FName("S3")); bCheckingFcat3 = false; }, 0.5f, false);
	}
	if (!bFcat4Exists && !bCheckingFcat4)
	{
		bCheckingFcat4 = true;
		GetWorldTimerManager().SetTimer(TimerHandle4, [this]() { DisplayMissingTag(FName("S4")); bCheckingFcat4 = false; }, 0.5f, false);
	}
}

void AJSH_Spawn::DisplayMissingTag(FName MissingTag)
{
	if (GEngine)
	{
		FString Message = FString::Printf(TEXT("Missing Tag: %s"), *MissingTag.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, Message);
	}
}
