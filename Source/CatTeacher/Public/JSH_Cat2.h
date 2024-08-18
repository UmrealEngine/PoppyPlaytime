// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JSH_Cat2.generated.h"

UCLASS()
class CATTEACHER_API AJSH_Cat2 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJSH_Cat2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// 적 AI관리 컴포넌트 클래스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSMcomponent)
	class UJSH_CatFSM2* fsm2;
};
