// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JSH_Switch.generated.h"

UCLASS()
class CATTEACHER_API AJSH_Switch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJSH_Switch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


	//UKMK_PlayerHandFSM* FSM;

	bool interaction = false;
	
	UPROPERTY()
	class UKMK_PlayerHandFSM* FSM;

	FString SwitchName;
	
	UPROPERTY()
	class UJSH_CatFSM* CatFsm;

	UPROPERTY()
	class AJSH_Steam* Steam01;
	UPROPERTY()
	class AJSH_Steam* Steam02;
	UPROPERTY()
	class AJSH_Steam* Steam03;
	UPROPERTY()
	class AJSH_Steam* Steam04;

	bool steam01 = false;
	bool steam02 = false;
	bool steam03 = false;
	bool steam04 = false;
};
