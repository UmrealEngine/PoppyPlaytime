// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class CATTEACHER_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 이미지 바인딩
	UPROPERTY(meta = (BindWidget))
	UImage* ArmImage;
	UPROPERTY(meta = (BindWidget))
	UImage* White1;
	UPROPERTY(meta = (BindWidget))
	UImage* White2;
	UPROPERTY(meta = (BindWidget))
	UImage* White3;

};
