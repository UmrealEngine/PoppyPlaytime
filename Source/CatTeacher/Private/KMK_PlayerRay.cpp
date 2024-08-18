// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_PlayerRay.h"
#include "KMK_PlayerHandFSM.h"
#include "KMK_Player.h"
#include "KMK_PlayerHand.h"
#include "../../../../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"
#include "KHH_EnemyFSM.h"
#include "Components/ArrowComponent.h"
#include "KMK_Bat.h"
#include "KMK_Battery.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UKMK_PlayerRay::UKMK_PlayerRay()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UKMK_PlayerRay::BeginPlay()
{
	Super::BeginPlay();
	FSM = GetOwner()->FindComponentByClass<UKMK_PlayerHandFSM>();
	playerComp = Cast<AKMK_Player>(GetOwner());
}


// Called every frame
void UKMK_PlayerRay::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 레이 셋팅
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());
	params.AddIgnoredActor(Hands[0]);
	params.AddIgnoredActor(Hands[1]);
	// 레이 쏘는 마지막 부분
	endPos = playerComp->endPos;

	// hit된 물체 정보 들고오기
	FHitResult hitInfo;
	//GetWorld()->LineTraceSingleByChannel(hitInfo, playerComp->startPos, endPos, ECC_GameTraceChannel8, params);
	// DrawDebugLine(GetWorld(), playerComp->startPos, endPos, FColor::Blue, false, 1.f);

	// 레이를 쏘고
	bool bhit = GetWorld()->LineTraceSingleByChannel(hitInfo, playerComp->startPos, endPos, ECC_GameTraceChannel8, params);
	if (isRay && startPoint < 0)
	{
		startPoint++;
		return;
	}
	if (hitInfo.GetActor() != nullptr && hitInfo.GetActor()->ActorHasTag("E_Bat"))
	{
		// 일정거리에 있을때 인터렉션이 가능하게 만듦
		if (FVector::Distance(hitInfo.GetActor()->GetActorLocation(), GetOwner()->GetTargetLocation()) < 400)
		{
			isCome = true;
			Hands[0]->isCome = true;
			Hands[1]->isCome = true;
		}
		// 벽에 있는 단자에 배터리가 들어있지 않은 경우
		auto* bat = hitInfo.GetActor()->FindComponentByClass<UKMK_Bat>();

		// 하고싶은 일 : 배터리를 넣었다 뺐다가 하게 하고 싶다
		// 해야하는 일
		// 1. 인터렉션이 가능하고
		if (isCome && isRay)
		{
			// 2. 단자에 배터리가 없을 경우
			if(!bat->isHaveBat)
			{ 
				// 2-1. 손에 배터리가 있다면
				for (int i = 0; i < 2; i++)
				{
					if (Hands[i]->isGrab)
					{
						// 2-2. 넣고 싶다
						bat->isHaveBat = true;
						// 2-3. 손에 있는 배터리를 끄고 싶다.
						Bats[i]->SetVis(false);
						// 2-4. 손이 grab상태가 아니게 하고 싶다
						Hands[i]->isGrab = false;
					}
				}
			}
			// 3. 단자에 배터리가 있는 경우
			else
			{
				if (bat->isHaveBat && isRay)
				{
					for (int i = 0; i < 2; i++)
					{
						if (playerComp->isDir[i])
						{
							// 3-1. 손에 배터리가 없을때
							
							if (!Hands[i]->isGrab)
							{
								// 3-2. 손에 배터리를 넣고 싶다.
								bat->isHaveBat = false;
								Bats[i]->SetVis(true);
								Hands[i]->isGrab = true;
							}
							// 4. 손에 배터리가 있을 때,
							else if (Hands[i]->isGrab)
							{
								// 4-1. 배터리를 떨어뜨리고 싶다.
								Bats[i]->SetVis(false);
								Hands[i]->isGrab = false;
								GetWorld()->SpawnActor<AKMK_Battery>(Hands[i]->BatteryFact, Hands[i]->trans);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		isCome = false;
		Hands[0]->isCome = false;
		Hands[1]->isCome = false;
	}

	// 입력값이 들어오는 경우
	if (isRay)
	{
	
		// 레이를 그리고
		// if(hitInfo.GetActor() != nullptr) DrawDebugLine(GetWorld(), startPos, endPos, FColor::Blue, false, 1.f);
		if (isCome)
		{
			isCome = false;
			return;
		}
		//  오른손 이라면
		if(playerComp->isDir[0])
		{	
			if(Hands[0]->isCome) Hands[0]->isCome = false;
			
			// 레이에 물체가 맞은 경우
			if(bhit)
			{
				// 총을 들고 있다면
				if (playerComp->RMeshComp->GetStaticMesh() == Hands[0]->HandMesh[1])
				{
					// 총을 쏜다
					FSM->isFire	= true;
					// FSM을 gunpack으로 변경하고
					FSM->PState = PlayerHandFSM::GunPack;
				}
				// 총을 들고있지 않다면
				else
				{	
					if (hitInfo.GetActor()->ActorHasTag("Jump") && FVector::Distance(hitInfo.GetActor()->GetActorLocation(), GetOwner()->GetActorLocation()) < 300)
					{
						Hands[0]->isJump = true;
					}

					// 오른손에 물체의 component를 넘겨주고
					Hands[0]->hitinfo = hitInfo.GetComponent();

					// 손을 뻗게 만든다
					// 손 초기값
					Hands[0]->handPos = 27;

                    if (hitInfo.GetActor()->GetActorLabel().Contains("wall") || hitInfo.GetActor()->GetActorLabel().Contains("Cube"))
                    {
                        Hands[0]->endPos = playerComp->startPos + playerComp->camera->GetForwardVector() * 500;
                    }
					else
					{
						// 손의 도착지점
						Hands[0]->endPos = hitInfo.ImpactPoint;
					}
					// 손을 뻗게 만듦
					if (Hands[0]->isGrab || Hands[0]->isPick)
					{
						Hands[0]->isGrab = false;
						Hands[0]->isPick = false;
					}
					else Hands[0]->isGo = true;
				}
			}
			// 물체가 맞지 않은 경우
			else
			{
				if (Hands[0]->isGrab)
				{
					Hands[0]->isGrab = false;
					return;
				}

				// 1. 총을 들고 있지 않다면
				if (playerComp->RMeshComp->GetStaticMesh() != Hands[0]->HandMesh[1])
				{
					// 손을 뻗는다
					Hands[0]->handPos = 27;
					if (hitInfo.GetActor() == nullptr)
					{
						Hands[0]->isGo = true;
						Hands[0]->endPos = playerComp->startPos + playerComp->camera->GetForwardVector() * 1000;
						return;
					}
					if (hitInfo.GetActor()->GetActorLabel().Contains("Wall") || hitInfo.GetActor()->GetActorLabel().Contains("Cube"))
					{
						Hands[0]->endPos = playerComp->startPos + playerComp->camera->GetForwardVector() * 1000;
					}
					else
					{
						// 손의 도착지점
						Hands[0]->endPos = hitInfo.ImpactPoint;
					}
					Hands[0]->isGo = true;
				}
				// 아니라면
				else
				{
					// 총쏘기
					FSM->isFire = true;
				}
			}

		}
		// 왼손이라면
		if (playerComp->isDir[1])
		{
			if (Hands[1]->isCome) Hands[1]->isCome = false;
			if (Hands[1]->isGrab)
			{
				Hands[1]->isGrab = false;
				return;
			}
			// 물체가 있다면
			if (bhit)
			{
				
				if (hitInfo.GetActor()->GetActorLabel().Contains("Wall") || hitInfo.GetActor()->GetActorLabel().Contains("Cube"))
				{
					Hands[1]->endPos = playerComp->startPos + playerComp->camera->GetForwardVector() * 1000;
				}
				else
				{
					Hands[1]->endPos = hitInfo.ImpactPoint;
				}
				Hands[1]->hitinfo = hitInfo.GetComponent();
			}
			else Hands[1]->endPos = playerComp->startPos + playerComp->camera->GetForwardVector() * 1000;

			// 공통적으로 수행
			Hands[1]->handPos = -27;
			Hands[1]->isRay = true;
			if (Hands[1]->isGrab || Hands[1]->isPick)
			{
				Hands[1]->isGrab = false;
				Hands[1]->isPick = false;
			}
			else Hands[1]->isGo = true;
		}
	}
}
// 레이 위치 생성
void UKMK_PlayerRay::SetRayPos(FVector start, FVector end)
{
	startPos = start;
	endPos = end;

	isRay = true;
}
