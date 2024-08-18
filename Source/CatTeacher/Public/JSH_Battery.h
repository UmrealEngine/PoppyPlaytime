// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JSH_Battery.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATTEACHER_API UJSH_Battery : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJSH_Battery();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	TArray<class AActor*> Steam;

	UPROPERTY()
	class UKMK_Bat* BatComponent03;
	UPROPERTY()
	class UKMK_Bat* BatComponent04;
	
	UPROPERTY()
	AActor* batsave03;
	UPROPERTY()
	AActor* batsave04;

	UPROPERTY()
	AActor* Cat03;
	
	UPROPERTY()
	AActor* Cat04;
	
	UPROPERTY()
	class UJSH_CatFSM* Cat03Component;
	
	UPROPERTY()
	class UJSH_CatFSM* Cat04Component;

	bool FSMOnOff03 = true;
	bool FSMOnOff04 = true;

	void FindTaggedActors();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool BPNiagaraOnOff = true;

	bool repeat03 = false;
	bool repeat04 = false;
};
