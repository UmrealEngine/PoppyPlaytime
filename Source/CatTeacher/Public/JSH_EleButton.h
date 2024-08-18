// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JSH_EleButton.generated.h"

UCLASS()
class CATTEACHER_API AJSH_EleButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJSH_EleButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY()
	class UKMK_PlayerHandFSM* FSM;

	UPROPERTY()
	class AJSH_Movebox* Elevator;

	UPROPERTY()
	class AJSH_Movebox* cage;

	bool Ofen = false;
	virtual void CageOfen();

	
	bool close = false;
	virtual void CageClose();

	
	bool upOn = false;
	virtual void EleUp();

	


	UPROPERTY(EditAnywhere)
	float distance = 600.0f;
	
	bool click02 = false;

	// 속도 조절
	UPROPERTY(EditAnywhere)
	float eleSpeed = 1.0f;


	UPROPERTY(EditAnywhere)
	float cageOftenSpeed = 1.0f;

	UPROPERTY(EditAnywhere)
	float cageCloseSpeed = 1.0f;


	float t = 0;
	
	UPROPERTY(EditAnywhere)
	float UpTime = 3;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget>widfact;

	UPROPERTY(EditAnywhere)
	class AActor* audioManager;

	bool MoveElv = true;
	bool MoveElv2=true;
};



