// Fill out your copyright notice in the Description page of Project Settings.

#include "KMK_PlayerHand.h"
#include "../../../../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"
#include "Components/ArrowComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/BoxComponent.h"
#include "KMK_Player.h"
#include "KMK_PlayerHandFSM.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/MovementComponent.h"
#include "KMK_Battery.h"
#include "Engine/HitResult.h"
#include "Components/PrimitiveComponent.h"
#include "KMK_Bat.h"
#include "KHH_BossOpendoor.h"
#include "PlayerAnimInstance.h"
#include "KHH_RotateDoor.h"
#include "PlayerWidget.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include <LeverComponent.h>
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

#include "JSH_Ending.h"
#include "JSH_EndingCat.h"
#include "KMK_FinalSwitch.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AKMK_PlayerHand::AKMK_PlayerHand()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
#pragma region Creaate
	// 손 만드는 생성자
	hand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandComp"));
	SetRootComponent(hand);
	hand->SetRelativeRotation(FRotator(0, -90, 0));
	hand->SetRelativeScale3D(FVector(2.f));
	hand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 손 앞에 arrow 만드는 생성자
	arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePos"));
	arrow->SetupAttachment(hand);
	arrow->SetRelativeLocationAndRotation(FVector(0, 10, 2.2f), FRotator(0, 90, 0));
	arrow->SetRelativeScale3D(FVector(0.2f));
	//=box collision
	box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	box->SetupAttachment(RootComponent);
	box->SetCollisionProfileName("Hand");
	box->SetRelativeLocation(FVector(0, 2, 2));
	box->SetBoxExtent(FVector(0.5f, 0.375, 0.5f));
	// 물건을 손에 집는 것 => 현재 사용 X
	handle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Handle"));
#pragma endregion

}

// Called when the game starts or when spawned
void AKMK_PlayerHand::BeginPlay()
{
	Super::BeginPlay();
	// 기본 설정
	player = Cast<AKMK_Player>(GetWorld()->GetFirstPlayerController()->GetPawn());
	hand->SetStaticMesh(HandMesh[0]);
	box->OnComponentBeginOverlap.AddDynamic(this, &AKMK_PlayerHand::BeginOverlap);
	box->BodyInstance.bUseCCD = true;
	box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 손에 달린 이펙트 담기
	VFXComp = FindComponentByClass< UNiagaraComponent>();
	if(VFXComp) VFXComp->SetVisibility(false);
	// 손 동적 material => 초록색 빛나게 하기
	myMatDynamic = UMaterialInstanceDynamic::Create(matFact, this);
	// material을 동적 material로 설정함
	if(matFact != nullptr) hand->SetMaterial(0, myMatDynamic);
	// 동적 material 내부에 있는 변수 조절 => 빛나지 X
	myMatDynamic->SetScalarParameterValue("charge_light", 0);


	// Ending을 위한
	EndingCat = Cast<AJSH_EndingCat>(UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_EndingCat::StaticClass()));
	EndingHelper = Cast<AJSH_Ending>(UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_Ending::StaticClass()));
}

// Called every frame
void AKMK_PlayerHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 손이 클릭됐는지 확인을 위함
	for (int i = 0; i < 3; i++)
	{
		if (player->isClick[i] && !player->bChangeHand)
		{
			return;
		}
	}

#pragma region isPick
	// 문 손잡이나 해치등을 잡고 있는 상태
	if (isPick)
	{
		// 딜라이트 맵의 레버용 if 문
		if(lever != nullptr && !lever->LeverMove) isPick = false;
		// GEngine->AddOnScreenDebugMessage(11, 5, FColor::Purple, FString::Printf(TEXT("%s"), *pickTrans.ToString()));
		// Collision을 켜준 상태
		box->SetCollisionProfileName("Hand");
		// 해치의 손잡이 등에 붙어있는 SceneComponent의 위치대로 이동시키기 위함
		hand->SetWorldLocation(pickTrans);
	}
	// 충전 단자에서 손이 조금 있다가 떨어지게 하기 위한 조건문과 for문
	if (isHold)
	{
		holdTime += DeltaTime;
		if (holdTime > HandTime)
		{
			holdTime = 0;
			isHold = false;
		}
	}
#pragma endregion

	// hatch를 위한 if문
	if (isDoor)
	{
		// 열리는 해치를 잡고있는 조건문
		if (isPick)
		{
			// rotDoor(해치에 달린 component)의 isOpen을 false로 만들어 해치가 열리지 않게 하기 위함
			rotDoor->isOpen = false;
			// 오른손인지 왼손인지 파악하기 위함
			if (GetName().Contains("R")) rotDoor->isRight = true;
			else rotDoor->isLeft = true;
			// 해치의 회전을 얼마나 할지 결정
			rot = FRotator(-50, 0, 0);
			// rotDoor의 RotateDoor1을 통해 해치를 닫음
			rotDoor->RotateDoor1(DeltaTime, rot, rotDoor->MoveTime);
			// 해치가 다 닫힌 경우
			if (rotDoor->GetOwner()->GetActorRotation().Pitch <= 0)
			{
				// 모든 함수들을 초기화
				isClosed = false;
				isDie = false;
				rotDoor->isRight = false;
				rotDoor->isLeft = false;
				isPick = false;
				isDoor = false;
				rotDoor->isComp = true;
				rotDoor->isOpen = false;
            }
		}
		// 만약 해치에서 손을 놓는 경우(다 안닫힌 경우)
		else if(!isPick && !rotDoor->isComp)
		{
			// 왼손인지 오른손인지 판별
			if (GetName().Contains("R")) rotDoor->isRight = false;
			else rotDoor->isLeft = false;
			// 해치가 열리게 하기
			rotDoor->isOpen = true;
			// 뭐였지...? 아마 손이 왼손 오른손 바꿔도 계속 유지되게 해주는 것...?
			isClosed = true;
			// 문이 다 열린경우
			if (rotDoor->GetOwner()->GetActorRotation().Pitch > 50)
			{
				// 다 초기화 시켜줌
				isClosed = false;
				isDie = true;
				isPick = false;
				isDoor = false;
				rotDoor->isOpen = false;
				rotDoor->isRight = false;
				rotDoor->isLeft = false;
				rotDoor->cnt = 0;
			}
		}
	}

#pragma region HandMove
	// 되돌아 올때는 나가지 못하게 막기
	if (isReverse)
	{
		isGo = false;
		if (!player->audioComps[4]->IsPlaying() && rCount <= 0)
		{
			rCount++;
			gCount = 0;
			UGameplayStatics::PlaySoundAtLocation(this, player->soundCue[4], GetActorLocation());
		}
	}
	// 나가는 상태일 때, 돌아오지 못하게 막음
	if (isGo)
	{
		isReverse = false;
        if (!player->audioComps[3]->IsPlaying() && gCount <= 0)
        {
			gCount++;
			rCount = 0;
			UGameplayStatics::PlaySoundAtLocation(this, player->soundCue[3], GetActorLocation());
        }
	}
	// 손이 돌아오는 곳
	if (isReverse && !isGo)
	{
		// 문을 잡고있는 경우가 아닌경우
		if(isHold) return;
		// 애니메이션 실행
		player->anim->PlayHandInMontage();
		// 방향 = 손의 원래 위치 - 현재 위치
		dir = startPos - GetActorLocation();
		// 거리확인
		float d = dir.Length();
		// GEngine->AddOnScreenDebugMessage(10, 1, FColor::White, FString::Printf(TEXT("%f"), d));
		dir.Normalize();
		// 이동
		SetActorLocation(GetActorLocation() + dir * speed * DeltaTime);
		// 원래 자리에 가까워지면
		if (d < 40.f)
		{
			// 1. 시간을 초기화 시키고
			t = 0;
			// 2. bool값을 초기화시켜 더이상 이동 못하게 막음
			isReverse = false;
			// 3. 레이도 못쏘게 막음
			isRay = false;
			// cable의 랜더링을 꺼줌
			for (int i = 0; i < 2; i++)
			{
				player->CableComp[i]->SetRenderInMainPass(false);
			}
			// 원래 위치로 초기화
			SetActorRelativeLocation(FVector(0));
			box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			// 총알 생성에 필요한 변수 초기화
			if (GetName().Contains("L") && player->FSM->cnt > 0) player->FSM->cnt = 0;
		}
	}

	// 손이 나가는 부분
	if (isGo && !isReverse)
	{	
		// 꺼준 케이블의 렌더링을 켜줌
		for (int i = 0; i < 2; i++)
		{
			player->CableComp[i]->SetRenderInMainPass(true);
		}
		player->anim->PlayHandMontage();
		// 손이 이동시에만 콜라이더 켜줌
		if (!isGrab)box->SetCollisionProfileName("Hand");
		// 일정시간이 지나거나 무언가를 잡고 있지 않는경우
		if (t > ShootTime && !isRay && !isGrab)
		{
			// 돌아오게 만듦
			isGo = false;
			isReverse = true;
		}
		// 시간 측정
		t += DeltaTime;
		// 방향 = 레이가 맞은 곳 - 손의 현재 위치
		dir = endPos - GetActorLocation();
		// 거리 측정 및 이동
		float distance = dir.Length();
		dir.Normalize();
		SetActorLocation(GetActorLocation() + dir * speed * DeltaTime);
		// 손이 레이의 거리보다 멀리 나가거나 목표지점에 도달한 경우
		if (distance > player->rayDis || distance < 40)
		{
			// 손이 돌아오게 만듦
			isGo = false;
			isReverse = true;
		}
	}
#pragma endregion
	// GEngine->AddOnScreenDebugMessage(3, 1, FColor::Orange, FString::Printf(TEXT("%d"), isCome));
#pragma region Battery Grab
	//
	// 손에 무언가를 잡고 있는 경우(배터리 등)
	if (isGrab )
	{
		// 콜라이더를 꺼주고
		box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 오른손 클릭이 들어온 경우
		if (player->isDir[0])
		{
			// 왼손 클릭이나 왼손이 무언가를 잡는 경우는 통과 못하게 만듦
			if (player->isDir[1] || player->Hands[1]->isGrab)
			{
				return;
			}
			// 손에 달린 배터리의 visible을 꺼주고 배터리를 월드에 생성시킴
			MakeBattery(0);

		}
		// 왼손 클릭이 들어온 경우
		if (player->isDir[1])
		{
			// 위와 같은 이유
			if (player->isDir[0] || player->Hands[0]->isGrab) return;
			// 손에 달린 visible을 꺼주고 배터리를 월드에 생성시킴
			MakeBattery(1);
		}
	}

#pragma endregion



}

// 손에 무언가 닿은경우
void AKMK_PlayerHand::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 돌아오게 만들기
	isGo = false;
	isReverse = true;
     if (bFromSweep)
     {
         isGo = false;
         isReverse = true;
     }

	// 뭔지 몰루
	SwitchName = *OtherActor->GetName();
	overActor = OtherActor;

	// 배터리인 경우
	if (OtherActor->ActorHasTag("Battery"))
	{
		// 오른손이 normal이 아닌 경우
		if (GetName().Contains("R") && player->RMeshComp->GetStaticMesh() != player->Hands[0]->HandMesh[0]) return;
		// grabActor = 배터리에 닮긴 component
		grabActor = Cast<AKMK_Battery>(OtherActor);
		// 이미 무언가를 집고 있는 경우, 제외하기 위함
		if (isGrab) return;
		// 잡은 상태로 만들어주고
		isGrab = true;
		// 닿았던 물체를 파괴시킴
		grabActor->Destroy();
		// 오른 손인 경우
		if (GetName().Contains("R"))
		{
			// 특정 손이 아니라면
			if (player->RMeshComp->GetStaticMesh() != player->Hands[0]->HandMesh[2])
			{
				// 손에 있는 배터리를 켜줌 
				player->Bats[0]->meshComp->SetRenderInMainPass(true);
				player->Bats[0]->meshComp->SetCollisionProfileName("Bat");
				isRight = true;
			}
		}
		// 왼손인 경우
		else
		{
			// 왼손에 있는 배터리를 켜줌
			isLeft = true;
			player->Bats[1]->meshComp->SetRenderInMainPass(true);
			player->Bats[1]->meshComp->SetCollisionProfileName("Bat");
		}
	}
	// 손잡이를 잡은경우
	if (OtherComp->ComponentHasTag("Handle"))
	{
		// 오른손 fsm에 따라 제어
		if (player->RMeshComp->GetStaticMesh() != player->Hands[0]->HandMesh[0] && GetName().Contains("R")) return;
		// 손잡이의 특정 위치 기억
		pickTrans = OtherComp->GetChildComponent(0)->GetComponentLocation();

		isPick = true;
		// 캣냅방 오른쪽 문
		if(OtherActor->FindComponentByClass<UKHH_BossOpendoor>() != nullptr)OtherActor->FindComponentByClass<UKHH_BossOpendoor>()->ShouldMove = true;
		// 해치의 경우
		if (OtherActor->FindComponentByClass<UKHH_RotateDoor>() != nullptr)
		{
			firstRot = OtherActor->GetActorRotation();
			rotDoor = OtherActor->FindComponentByClass<UKHH_RotateDoor>();
			isDoor = true;
		}
		// 레버의 경우
		lever = OtherActor->FindComponentByClass<ULeverComponent>();
		if(lever != nullptr)
		{
			leverRot = OtherActor->GetActorRotation();
			lever->LeverMove = true;
			isLever = true;
		}
	}
	// 왼손이면 아래로 내려가지 못하게 막기
	if(!GetName().Contains("R")) return;
	// 점프패드인 경우
	if (isJump && GetName().Contains("R"))
	{
		if (OtherActor->ActorHasTag("Jump") && player->RMeshComp->GetStaticMesh() == player->Hands[0]->HandMesh[2])
		{
			// 점프 상태로 변경
			FSM->isJump = true;
			isJump = false;
			FSM->PState = PlayerHandFSM::JumpPack;
		}
	}
	else
	{
			// 콜리전 꺼주기
			box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Charge가 가능한 패널인 경우
	if (player->RMeshComp->GetStaticMesh() == player->Hands[0]->HandMesh[0] && OtherActor->ActorHasTag("ElectricalPanel") && GetName().Contains("R"))
	{
		// 충전 상태로 변경하고 모든 material을 켜줌
		FSM->PState = PlayerHandFSM::Energy;
		FSM->t = 0;
		FSM->isCharge = true;
		myMatDynamic->SetScalarParameterValue("charge_light", 4);
		VFXComp->SetVisibility(true);
		isHold = true;
		UGameplayStatics::PlaySoundAtLocation(this, player->soundCue[5], GetActorLocation());
		// Final 스위치인 경우
		if (OtherActor->FindComponentByClass<UKMK_FinalSwitch>())
		{

			GEngine->AddOnScreenDebugMessage(9, 1, FColor::Emerald, FString::Printf(TEXT("complete")));
		}
	}

	if(OtherActor->ActorHasTag("EndingSwitch"))
	{
		EndingCat->EndingVisible = true;
		EndingHelper->EndingStart = true;
	}
	

}

void AKMK_PlayerHand::MakeBattery(int32 num)
{
	// 손의 위치
	trans = player->Bats[num]->GetTransform();
	// 손에 달린 배터리 꺼줌
	player->Bats[num]->SetVis(false);

	// 배터리 생성
	if (!isCome && !isPick)
	{
		GetWorld()->SpawnActor<AKMK_Battery>(BatteryFact, trans);
	}
}
