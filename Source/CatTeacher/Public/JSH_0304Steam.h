// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "JSH_0304Steam.generated.h"

UCLASS()
class CATTEACHER_API AJSH_0304Steam : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJSH_0304Steam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ¿¬±â
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* NSteam;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSMcomponent)
	class UJSH_Battery* batfsm;
};
