// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KMK_FinalSwitch.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UKMK_FinalSwitch : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKMK_FinalSwitch();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnyWhere)
	TArray<AActor*> bats;

	UPROPERTY(VisibleAnywhere)
	TArray<class UKMK_Bat*> batsComps;

	TArray<bool> isHave = {false, false, false, false};
	int count = 0;
	float allCharge = 0;

	UPROPERTY(EditAnywhere)
	float playerDist = 800;
	
	UPROPERTY(EditAnywhere)
    class UMaterialInstance* matFact;

	UPROPERTY()
	UMaterialInstanceDynamic* myMatDynamic;

	UPROPERTY()
	class UStaticMeshComponent* meshComp;

	float t = 0;
	bool isTrue = false;
	float gage = 100;
	float page = 0;


	// CatNab
	UPROPERTY()
	class AJSH_Random* RandomTag;
};
