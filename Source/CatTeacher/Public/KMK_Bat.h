// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KMK_Bat.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UKMK_Bat : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKMK_Bat();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 배터리가 들어왔는지 확인하기 위한 변수
	UPROPERTY(EditAnywhere)
	bool isHaveBat = false;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* meshBat;
	UPROPERTY()
	class UKMK_PlayerRay* player;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battery", meta = (AllowPrivateAccess = "true"))

	// 방법 1-2. 직접할당을 하겠다 :
    AActor* TargetDoor; 
	UFUNCTION()
    void SetTargetDoor(AActor* NewTargetDoor);

	UPROPERTY()
	class UKHH_BatteryOpenDoor* BatteryOpendoorComponent;

	UPROPERTY(EditAnywhere)
	class AStaticMeshActor* batProgress;
	
	UPROPERTY(EditAnywhere, Category = "Progress")
	float ChargeSpeed  = 0.02f;
	float spd =0;
	
	UPROPERTY(EditAnywhere)
    class UMaterialInstance* matFact;
	UPROPERTY(VisibleAnywhere)
	class UMaterialInstanceDynamic* myMatDynamic;

	void ChargeGage(float DeltaTime, int count);

	bool isStart = false;
};
