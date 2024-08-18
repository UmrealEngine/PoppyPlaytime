// Fill out your copyright notice in the Description page of Project Settings.


#include "KHH_BossOpendoor.h"
#include "KMK_Player.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values for this component's properties
UKHH_BossOpendoor::UKHH_BossOpendoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKHH_BossOpendoor::BeginPlay()
{
	Super::BeginPlay();
	OriginalLocation = GetOwner()->GetActorLocation();
}


// Called every frame
void UKHH_BossOpendoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Player = Cast<AKMK_Player>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (Player && ShouldMove)
	{
		CurrentLocation = GetOwner()->GetActorLocation();
		TargetLocation = OriginalLocation + MoveOffset;
		Speed = FVector::Distance(OriginalLocation, TargetLocation) / MoveTime;
		NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
		GetOwner()->SetActorLocation(NewLocation);

		if (!BOD) {
			BOD = true;
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), BossDoorSound, GetOwner()->GetActorLocation());
		}

	} else return;
}

