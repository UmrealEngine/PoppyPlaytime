// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KHH_Switch.generated.h"

UCLASS()
class CATTEACHER_API AKHH_Switch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKHH_Switch();
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;


	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
		
	UPROPERTY(EditAnywhere)
	AActor* door;

	class UKHH_BatteryOpenDoor* BatteryOpendoorComponent;

	class UKMK_PlayerHandFSM* FSM;

	void Opendoor();

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	UPROPERTY(EditAnywhere)
	bool lastDoor = false; 
};