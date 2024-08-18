// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JSH_RandomFalse.generated.h"

UCLASS()
class CATTEACHER_API AJSH_RandomFalse : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJSH_RandomFalse();

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
	float FalseRandomeTime = 15.0f;
	
};
