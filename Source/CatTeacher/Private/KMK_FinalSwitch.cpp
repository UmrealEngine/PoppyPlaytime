// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_FinalSwitch.h"

#include "JSH_Random.h"
#include "KMK_Bat.h"
#include "KMK_Player.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"

// Sets default values for this component's properties
UKMK_FinalSwitch::UKMK_FinalSwitch()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// begin Play는 신경쓰지 마세요 기본 셋팅들 해놓은 것 => 주변 단자랑 마지막 스위치랑 연결하기 위한 것들이여서 신경 안써도돼
 
// UKMK_FinalSwitch는 현재 5개의 Actor를 받아서 처리중입니다
// (인덱스 기준) 0번부터 3번까지는 단자 Actor를 가져온 상태이고 4번은 컴퓨터 상단에 있는 스위치 관련된 친구입니다.

// 현재 로직
// 여기에 있는 bats = 위에서 이야기한 5개의 Actor를 받아오는 상태 => 직접 할당
// batsComps은 단자에 component에 달려있는 UKMK_Bat를 받아오는 변수
// UKMK_Bat 내부에 있는 isHaveBat(배터리를 가지고 있는지 확인하는 변수)를 확인하고 같은 컴포넌트에 있는 ChargeSpeed(단자마다 달린 프로그래스바) 변수를 통해 컴퓨터 프로그래스바를 채우는 형태입니다

void UKMK_FinalSwitch::BeginPlay()
{
	Super::BeginPlay();
	bats[4]->SetActorEnableCollision(false);
	for(int i = 0; i < 4; i++)
	{
		batsComps.Add(bats[i]->FindComponentByClass<UKMK_Bat>());
	}
	meshComp = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	myMatDynamic = UMaterialInstanceDynamic::Create(matFact, this);
	if (matFact != nullptr && meshComp != nullptr) meshComp->SetMaterial(0, myMatDynamic);
	myMatDynamic->SetScalarParameterValue("Page", 0);
	myMatDynamic->SetScalarParameterValue("Gage", 100);

	// CatNab RandomTag 
	RandomTag = Cast<AJSH_Random>(UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_Random::StaticClass()));
}


// Called every frame
void UKMK_FinalSwitch::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto* player = Cast<AKMK_Player>(GetWorld()->GetFirstPlayerController()->GetPawn());
	float d = FVector::Distance(player->GetActorLocation(), GetOwner()->GetTargetLocation());
	//GEngine->AddOnScreenDebugMessage(5, 1, FColor::White, FString::Printf(TEXT("%f"), d));
	// 캣냅 시작 전, 모니터 안내 문구 관련 if 문 => PlayerDist를 통해 모니터와 플레이어의 간격 설정 가능함 
	// isTrue = true인 경우 캣냅 fsm이 시작되어야함
	if (!isTrue && d < playerDist)
	{
		// 일정시간이 지나면
		t += DeltaTime;
		if (t > 5)
		{
			// 스크린 페이지가 변경되고
			myMatDynamic->SetScalarParameterValue("Page", 1);
			if (t > 3.f)
			{
				// 스크린의 게이지가 0으로 초기화됨
				t = 0;
				isTrue = true;
				myMatDynamic->SetScalarParameterValue("Gage", 0);

				// CatNab 공격 상태를 변경해주는 로직 시작
				RandomTag->start = true;
				UE_LOG(LogTemp, Warning, TEXT("FSM Start"));
			}
		}
	}
	// 마지막 스위치랑 연결된 단자들이 캣냅 기믹이 시작될때 시작하기 위해 넣은 변수임
	if (isTrue)
	{
		for (int i = 0; i < 4; i++)
		{
			batsComps[i]->isStart = true;
		}
	}
	if(!isTrue) return;
	// 캣냅 기믹이 시작되는 부분
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Printf(TEXT("%f"), allCharge));
	//GEngine->AddOnScreenDebugMessage(6, 1, FColor::Yellow, FString::Printf(TEXT("%d"), count));
	// 4개 단자에서 모든 게이지를 얻는 경우
	if (allCharge > 100 || (player != nullptr && player->isCheat2))
	{
		allCharge = 100;
		// 컴퓨터 상단 스위치에 달린 collision을 켜줌
		bats[4]->SetActorEnableCollision(true);
	}
	// 게이지를 얻고 있다면
	else
	{
		/* Material instance의 경우, Page(페이지 변경)과 Gage(프로그래스바) 2가지의 변수가 존재함 
			=> 해당 Material instance는 8/12에 TA쪽에서 만들어서 올려놨다고 함
			=> 위치 경로는 직접 확인해보면 됩니다 */
		// 가운데 프로그래스 바에 있는 material instance의 변수 중 gage 부분을 allCharge(단자에서 얻어오는 값)을 변경하겠다
		myMatDynamic->SetScalarParameterValue("Gage", allCharge);
		
		// 4가지의 단자 상태를 확인하기 위한 for문
		for (int i = 0; i < 4; i++)
		{
			// 단자에 붙은 component(KMK_Bat) 내부의 isHaveBat를 통해 단자에 배터리가 있는지 확인함
			// 배터리가 있는 경우
			if (batsComps[i]->isHaveBat && !isHave[i])
			{
				count++;
				isHave[i] = true;
				
			}
			// 배터리가 없는 경우 
			if (!batsComps[i]->isHaveBat && isHave[i])
			{
				count--;
				isHave[i] = false;
			}

		}

		// 배터리가 1개랃도 존재하면
		if (count != 0)
		{
			if (allCharge >= 25)
			{
				batsComps[1]->ChargeGage(batsComps[1]->ChargeSpeed, count + 1);
			}
			// 배터리가 없어도 일정량이 컴퓨터에 충전되면 단자에 붙은 프로그래스바가 올라가게 만들기 위함
			if (allCharge >= 50)
			{
				batsComps[2]->ChargeGage(batsComps[2]->ChargeSpeed, count + 2);
			}
			if (allCharge >= 75)
			{
				batsComps[3]->ChargeGage(batsComps[3]->ChargeSpeed, count + 3);
			}
			// 컴퓨터 내부의 있는 프로그래스 바의 비율 변경해줌
			allCharge += batsComps[0]->ChargeSpeed * 0.25f * count;
		}
	}
}

