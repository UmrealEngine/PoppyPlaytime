// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFX_Manager.generated.h"

UCLASS()
class CATTEACHER_API ASFX_Manager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASFX_Manager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// BGM - 레벨마다 세팅
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGM")
	USoundBase* BGM;
	UPROPERTY()
	UAudioComponent* audio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGM")
	
	bool BGMmanager = false;
	int BGMCount = 0;
};
