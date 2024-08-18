// Fill out your copyright notice in the Description page of Project Settings.


#include "JSH_Cat.h"
#include "JSH_CatFSM.h"
#include "JSH_Ending.h"
#include "KMK_Bullet.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AJSH_Cat::AJSH_Cat()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// EnemyFSM 컴포넌트 추가
	fsm = CreateDefaultSubobject<UJSH_CatFSM>(TEXT("FSM"));

	// FalseCat Collision 설정 전 , FSM에서 Collision 변경 예정
	FalseBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FalseBox"));
	FalseBox->SetupAttachment(RootComponent);
	FalseBox->SetCollisionProfileName(TEXT("NoCollision"));
	FalseBox->SetWorldScale3D(FVector(7.0f, 7.0f, 7.0f));
	FalseBox->SetRelativeLocation(FVector(80.0f, 0.0f, 170.0f));
}

// Called when the game starts or when spawned
void AJSH_Cat::BeginPlay()
{
	Super::BeginPlay();

	WidgetPlay = false;

	EndingHelper = Cast<AJSH_Ending>(UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_Ending::StaticClass()));
}

void AJSH_Cat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayAttackAnimation)
	{
		HandDel = true;
		
		if(ccamera)
		{
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(ccamera);
		}
		
		EndingHelper->JumpScareEnding = true;
		// Load the animation sequence
		static USoundWave* AttackSound = LoadObject<USoundWave>(nullptr, TEXT("/Game/Project/JSH/Audio/Sw_catnap_jumpscare.Sw_catnap_jumpscare"));
		static UAnimSequence* AttackAnim = LoadObject<UAnimSequence>(nullptr, TEXT("/Game/Project/JSH/Asset/4/source/Armature_A_NappyCat_Jumpscare_Normal.Armature_A_NappyCat_Jumpscare_Normal"));
		currtime += DeltaTime;
		if (AttackAnim)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackSound, GetActorLocation());
			timestart = true;
			// Play the animation on the mesh
			GetMesh()->PlayAnimation(AttackAnim, false);
			// 스켈레톤 문제로 , 몽타주 노티파이를 찍을 수 가 없어서 , animation 실행 시간으로 대체 해야 할듯
			GEngine->AddOnScreenDebugMessage(28, 3, FColor::Green, FString::Printf(TEXT("animation")));
			PlayAttackAnimation = false;
		}
	}
	if (timestart)
	{
		currtime += DeltaTime;
		if (currtime >= aniendtime)
		{
			// APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			//
			// // Check if PlayerController is valid
			// if (PlayerController)
			// {
			// 	// Exit the game
			// 	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
			// }
			WidgetPlay = true;
			GEngine->AddOnScreenDebugMessage(28, 3, FColor::Green, FString::Printf(TEXT("game end")));
			timestart = false;
			currtime = 0;
		}
	}
}

// Called to bind functionality to input
void AJSH_Cat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AJSH_Cat::NotifyActorBeginOverlap(AActor* otherActor)
{
	Super::NotifyActorBeginOverlap(otherActor);

	AKMK_Bullet* Bullet = Cast<AKMK_Bullet>(otherActor);
	if (Bullet)
	{
		//fsm->FalseDie = true;
		fsm->IdleTeleport = true;
		fsm->cState = ECatState::Idle;
		//Destroy(Bullet);
	}
}