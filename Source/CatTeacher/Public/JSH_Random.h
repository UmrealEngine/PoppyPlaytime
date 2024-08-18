// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JSH_Random.generated.h"

UCLASS()
class CATTEACHER_API AJSH_Random : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJSH_Random();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	// 타이머 핸들
	FTimerHandle TimerHandle;

	// 태그를 찾고 랜덤으로 선택하는 함수
	void FindAndSelectRandomTag();



public:
	UPROPERTY(EditAnywhere, Category = "Control")
	float TrueRandomeTime = 10.0f;


	// 티비 화면에 맞춰서 Tag 뽑기를 시작하는 함수 
	void ss();

	// JumpScare나 Endig때에 Tag 뽑기를 멈추는 함수
	void StopRandomTagSelection();
	
	UPROPERTY(EditAnywhere)
	bool start = false;


	UPROPERTY(EditAnywhere)
	bool Stop = false;

	
	UPROPERTY()
	UAudioComponent* BasicBGM;

	UPROPERTY()
	UAudioComponent* DangerBGM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EndingSoundOff = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AJSH_EndingCat* EndingCat;
};
