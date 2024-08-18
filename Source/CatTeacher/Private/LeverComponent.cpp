// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverComponent.h"
#include "KMK_Player.h"
#include "LeverAnimInstance.h"
#include "KHH_Enemy.h"
#include "KHH_BossOpendoor.h"

// Sets default values for this component's properties
ULeverComponent::ULeverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULeverComponent::BeginPlay()
{
	Super::BeginPlay();
	InitialRotation = GetOwner()->GetActorRotation();
	// ...
    //Anim = Cast<ULeverAnimInstance>(me->GetMesh()->GetAnimInstance());

}


// Called every frame
//void ULeverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	if(me)GEngine->AddOnScreenDebugMessage(10, 1, FColor::Magenta, FString::Printf(TEXT("find!")));
//	//if (leverAnim->LeverMove)
//
//	
//	if (LeverMove)
//	{
//		CurrentRotation = GetOwner()->GetActorRotation();
//		TargetRotation = InitialRotation + OpenAngle;
//		Speed = FRotator::NormalizeAxis((TargetRotation - InitialRotation).Roll) / MoveTime;
//		if (Speed < 0)
//		{
//			Speed *= -1;
//		}
//		NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, Speed);
//		GetOwner()->SetActorRotation(NewRotation);
//	}
//}

void ULeverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (LeverMove)
    {
        CurrentRotation = GetOwner()->GetActorRotation();
        TargetRotation = InitialRotation + OpenAngle;
        me->isRun = true;
        Speed = FRotator::NormalizeAxis((TargetRotation - InitialRotation).Roll) / MoveTime;
        if (Speed < 0)
        {
            Speed *= -1;
        }
        NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, Speed);
        GetOwner()->SetActorRotation(NewRotation);

        if (GetOwner()->GetActorRotation().Roll >= 80 )
        {
            GetOwner()->SetActorRotation(TargetRotation);
            LeverMove = false;
        }
        if (Targetshutter != nullptr)SetTargetShutter(Targetshutter);

    }
}

void ULeverComponent::SetTargetShutter(AActor* NewTargetDoor)
{
    if (NewTargetDoor != nullptr)
    {	
        BossOpendoorComponent= NewTargetDoor->FindComponentByClass<UKHH_BossOpendoor>();
        if (BossOpendoorComponent)
        {
            BossOpendoorComponent->ShouldMove = true;
        }
    }
}


