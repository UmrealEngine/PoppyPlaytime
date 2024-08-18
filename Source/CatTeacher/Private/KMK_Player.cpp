// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_Player.h"

#include "InstancedReferenceSubobjectHelper.h"
#include "KMK_PlayerHandFSM.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "Engine/GameEngine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "KMK_PlayerRay.h"
#include "Components/ArrowComponent.h"
#include "KMK_PlayerHand.h"
#include "../../../../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "KMK_Battery.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "KHH_Enemy.h"
#include "PlayerAnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.h"
#include "Components/Image.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

// Sets default values
AKMK_Player::AKMK_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 1. 1인칭 카메라 만들기
	// 1-1 spring arm 붙이기
	FString s;
	float y;
	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			s = "R";
			y = 24;
		}
		else
		{
			s = "L";
			y = -24;
		}
		FString armName = FString::Printf(TEXT("SprintArm %i"), i);
		springArm = CreateDefaultSubobject<USpringArmComponent>(*armName);

		springArms.Add(springArm);
		springArms[i]->TargetArmLength = -50.f;
		springArms[i]->bUsePawnControlRotation = true;
		FString CableName = FString::Printf(TEXT("%s Cable"), *s);
		CableComp.Add(CreateDefaultSubobject<UCableComponent>(*CableName));
		CableComp[i]->CableLength = 1;
		CableComp[i]->NumSegments = 1;
		CableComp[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CableComp[i]->CableWidth = 3;
		FString SceneName = FString::Printf(TEXT("%s Scene"), *s);
		SceneComp.Add(CreateDefaultSubobject<USceneComponent>(*SceneName));
	}
#pragma region Modeling
	springArms[0]->SetupAttachment(GetMesh());
	springArms[0]->SetRelativeLocation(FVector(0, 0, 140));
	// 팔이 카메라보다 늦게 쫓아오게 만들기 위함
	springArms[0]->bEnableCameraLag = true;
	springArms[0]->bEnableCameraRotationLag = true;
	springArms[0]->CameraLagSpeed = 15.f;
	springArms[0]->CameraRotationLagSpeed = 15.f;
	springArms[0]->CameraLagMaxDistance = 3;

	// 팔 매쉬 붙이기
	armMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GrabpackMesh"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Project/KMK/Mod/source/SK_FirstPersonPlayer_Grabpack2.SK_FirstPersonPlayer_Grabpack2'"));
	if (tempMesh.Succeeded())
	{
		armMesh->SetSkeletalMesh(tempMesh.Object);
	}
	armMesh->SetupAttachment(springArms[0]);
	armMesh->SetRelativeRotation(FRotator(0, -90, 0));
	armMesh->SetRelativeScale3D(FVector(1.8f));

#pragma endregion
	// 팔
//GrabSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("GrabSpringArm"));

	springArms[1]->SetupAttachment(GetMesh(), "head");
	springArms[1]->SetRelativeLocation(FVector(-20, 0, 0));

	// 1-2 camera 붙이기
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArms[1]);
	camera->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = true;
	// 1-3 모델링 설정

	// 점프 횟수 제한
	JumpMaxCount = 1;
	// FSM 붙이기
	FSM = CreateDefaultSubobject<UKMK_PlayerHandFSM>(TEXT("FSM"));
	playerRay = CreateDefaultSubobject<	UKMK_PlayerRay>(TEXT("RAY"));
	// 센서 만들기
	sensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Sensor"));
	sensor->bOnlySensePlayers = false;
}

// Called when the game starts or when spawned
void AKMK_Player::BeginPlay()
{
	Super::BeginPlay();
	// 위잿 관련 스타트
	widget = Cast<UPlayerWidget>(CreateWidget(GetWorld(), WidFact));
	if (widget)
	{
		widget->AddToViewport(1);

	}
#pragma region Create Hand
	float y;
	float x;
	FString s;
	TSubclassOf<AKMK_PlayerHand> hand;
	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			s = "R";
			x = -14.f;
			y = 24;
			hand = RHandFact;
		}
		else
		{
			s ="L";
			x = 14.f;
			y = -24;
			hand = LHandFact;
		}
		FString armName = FString::Printf(TEXT("%sHand"), *s);
		Hands.Add(GetWorld()->SpawnActor<AKMK_PlayerHand>(hand));
		Hands[i]->AttachToComponent(armMesh, FAttachmentTransformRules::KeepRelativeTransform, *armName);
		Hands[i]->FSM = FSM;
		Hands[i]->SetActorRelativeLocation(FVector(0));
		Hands[i]->SetActorRelativeRotation(FRotator(0));
		playerRay->Hands.Add(Hands[i]);

		CableComp[i]->AttachToComponent(armMesh, FAttachmentTransformRules::KeepRelativeTransform, *armName);
		CableComp[i]->SetRelativeLocationAndRotation(FVector(0), FRotator(0, 0, 0));
		CableComp[i]->SetAttachEndTo(Hands[i], NAME_None);

		SceneComp[i]->AttachToComponent(armMesh, FAttachmentTransformRules::KeepRelativeTransform, *armName);
		SceneComp[i]->SetRelativeLocationAndRotation(FVector(0), FRotator(0, 0, 0));


		FTransform t = SceneComp[i]->GetRelativeTransform();
		Bats.Add(GetWorld()->SpawnActor<AKMK_Battery>(batteryFact, t));
		Bats[i]->meshComp->SetSimulatePhysics(false);
		Bats[i]->meshComp->SetEnableGravity(false);
		Bats[i]->AttachToComponent(armMesh, FAttachmentTransformRules::KeepRelativeTransform);
		Bats[i]->SetActorRelativeLocation(FVector(x, 30, 8));
		Bats[i]->SetActorScale3D(FVector(0.7f));
		Bats[i]->meshComp->SetRenderInMainPass(false);
		Bats[i]->meshComp->SetRenderInDepthPass(false);
		auto* b = Bats[i]->FindComponentByClass<UBoxComponent>();
		b->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Bats[i]->meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		playerRay->Bats.Add(Bats[i]);
	}
	
	RMeshComp = Hands[0]->hand;
#pragma endregion

	movementComp = GetCharacterMovement();
	jumpPower = movementComp->JumpZVelocity;
	auto* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Mapping, 0);
		}
	}
	movementComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	// 애니메이션 관련 작업
	anim = Cast<UPlayerAnimInstance>(armMesh->GetAnimInstance());
	// 머테리얼 변경
	myMatDynamic = UMaterialInstanceDynamic::Create(matFact, this);

	myMatDynamic->SetScalarParameterValue("Gauge", gauzeSpd);
	widget->ArmImage->SetBrushFromMaterial(myMatDynamic);
	
	whiteHand.Add(widget->White1);
	whiteHand.Add(widget->White2);
	whiteHand.Add(widget->White3);
	
	for (int i = 0; i < 3; i++)
	{
		whiteHand[i]->SetColorAndOpacity(FLinearColor(1, 1, 1, 0));
	}

	for (int i = 0; i < soundCue.Num(); i++)
	{
		UAudioComponent* newAudioComp = NewObject<UAudioComponent>(this);
		audioComps.Add(newAudioComp);
		audioComps[i]->bAutoActivate = false;
		audioComps[i]->SetupAttachment(RootComponent);
		audioComps[i]->RegisterComponent();

		if (audioComps[i] != nullptr)
		{
			audioComps[i]->SetSound(soundCue[i]);
		}
	}
    audioComps[0]->SetPitchMultiplier(walkSpeed);
    audioComps[1]->SetPitchMultiplier(RunSpeed);
}

// Called every frame
void AKMK_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (anim->isNormal)
	{
		Hands[0]->hand->SetStaticMesh(HandMesh);
		Hands[0]->hand->SetMaterial(0, Hands[0]->myMatDynamic);
		anim->isNormal = false;
	}
	for (int i = 0; i < 3; i++)
	{
		if (isOn[i])
		{
			disTime -= DeltaTime * disSpeed;
			whiteHand[i]->SetColorAndOpacity(FLinearColor(1, 1, 1, disTime));
			if (disTime < 0)
			{
				isOn[i] = false;
				disTime = 1;
				whiteHand[i]->SetColorAndOpacity(FLinearColor(1, 1, 1, 0));
			}
		}
	}
	for (int i = 0; i < 2; i++)
	{
		FString s;
		if (i == 0)
		{
			s = "R";
		}
		else
		{
			s = "L";
		}
		Bats[i]->hand = Hands[i];
		FString name = FString::Printf(TEXT("%sHand"), *s);
		Hands[i]->startPos = armMesh->GetSocketLocation(*name);
	}

	// 이동하기
	// 회전방향으로 이동
	// 1. controller rotation을 통해 transform 생성
	FSM->bulletTrans = Hands[0]->arrow->GetComponentTransform();
	FTransform t = FTransform(GetControlRotation());
	dir = t.TransformVector(dir);
	// 이동 인풋
	AddMovementInput(dir, speed * DeltaTime);
	dir = FVector::ZeroVector;
	// 계속 레이 쏘기
	startPos = camera->GetComponentLocation();
	endPos = startPos + camera->GetForwardVector() * rayDis;
	endPos1 = startPos + camera->GetForwardVector() * rayDis1;


	// 사운드 재생

	float SFXSpd = GetVeloFunc();
	if (SFXSpd > 0 && SFXSpd < 800)
	{
		isSFX[0] = true;
		isSFX[1] = false;
	}
	else if (SFXSpd >= 800)
	{
		isSFX[0] = false;
		isSFX[1] = true;
	}
	else
	{
		isSFX[0] = false;
		isSFX[1] = false;
	}

	if (movementComp->IsFalling())
	{
		isSFX[0] = false;
		isSFX[1] = false;

	}
	// 게이지바 변경
	for (int i = 0; i < 2; i++)
	{

		if (Hands[i]->isGo)
		{
			gauzeSpd -= 0.03;
			if(gauzeSpd < 0) gauzeSpd = 0;

			myMatDynamic->SetScalarParameterValue("Gauge", gauzeSpd);
		}

		if (Hands[i]->isReverse)
		{
			gauzeSpd += 0.03 * 3;
			if (gauzeSpd > 1) gauzeSpd = 1;
			myMatDynamic->SetScalarParameterValue("Gauge", gauzeSpd);
		}
		if (isSFX[i])
		{
			if (!audioComps[i]->IsPlaying())
			{
				PlayPlayerSound(i);
			}
		}
		else
		{
			if (audioComps[i]->IsPlaying())
			{
				StopPlayerSound(i);
			}
		}
	}


}

#pragma region Audio
void AKMK_Player::PlayPlayerSound(int index)
{
	if (audioComps[index] && !audioComps[index]->IsPlaying())
	{
		audioComps[index]->Play();
	}
}

void AKMK_Player::StopPlayerSound(int index)
{
	if (audioComps[index] && audioComps[index]->IsPlaying())
	{
		audioComps[index]->Stop();
	}
}

float AKMK_Player::GetVeloFunc()
{
	return GetVelocity().Size();
}


void AKMK_Player::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	isSFX[0] = false;
	isSFX[1] = false;

	// 착지 확인
	UGameplayStatics::PlaySoundAtLocation(this, soundCue[2], GetActorLocation());
}

#pragma endregion
// Called to bind functionality to input
void AKMK_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// 플레이어 인풋 처리
	auto playerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (playerInput)
	{
		// 키 바인딩
		// 회전 바인딩
		playerInput->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AKMK_Player::InputTurn);
		// 움직임 바인딩
		playerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AKMK_Player::InputMove);
		// 점프 바인딩
		playerInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &AKMK_Player::InputJump);
		// 앉기 바인딩
		playerInput->BindAction(IA_Sit, ETriggerEvent::Started, this, &AKMK_Player::InputSit);
		playerInput->BindAction(IA_Sit, ETriggerEvent::Completed, this, &AKMK_Player::InputStand);
		// 달리기 바인딩
		playerInput->BindAction(IA_Run, ETriggerEvent::Started, this, &AKMK_Player::InputRun);
		playerInput->BindAction(IA_Run, ETriggerEvent::Completed, this, &AKMK_Player::InputWalk);

		// 손 변경 바인딩
		playerInput->BindAction(IA_Click1, ETriggerEvent::Started, this, &AKMK_Player::InputNum1);
		playerInput->BindAction(IA_Click2, ETriggerEvent::Started, this, &AKMK_Player::InputNum2);
		playerInput->BindAction(IA_Click3, ETriggerEvent::Started, this, &AKMK_Player::InputNum3);
		// 마우스 클릭
		// 오른손
		playerInput->BindAction(IA_ClickR, ETriggerEvent::Started, this, &AKMK_Player::InputMR);
		// playerInput->BindAction(IA_ClickR, ETriggerEvent::Canceled, this, &AKMK_Player::InputMRComp);
		playerInput->BindAction(IA_ClickR, ETriggerEvent::Completed, this, &AKMK_Player::InputMRComp);
		// 왼손
		playerInput->BindAction(IA_ClickL, ETriggerEvent::Started, this, &AKMK_Player::InputML);
		// playerInput->BindAction(IA_ClickL, ETriggerEvent::Canceled, this, &AKMK_Player::InputMLComp);
		playerInput->BindAction(IA_ClickL, ETriggerEvent::Completed, this, &AKMK_Player::InputMLComp);
#pragma region cheat
		// 이스터에그
		//playerInput->BindAction(IA_ClickE, ETriggerEvent::Started, this, &AKMK_Player::InputE);
		// 딜라이트
		// playerInput->BindAction(IA_Cheat1, ETriggerEvent::Started, this, &AKMK_Player::Cheat1);
		// 캣냅
		playerInput->BindAction(IA_Cheat2, ETriggerEvent::Started, this, &AKMK_Player::Cheat2);
#pragma endregion

	}

}

#pragma region MouseRotation
// 회전처리 함수
void AKMK_Player::InputTurn(const struct FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	AddControllerPitchInput(-v.Y);
	AddControllerYawInput(v.X);
}
#pragma endregion
#pragma region Move
// 이동관련
void AKMK_Player::InputMove(const struct FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	dir.X = v.X;
	dir.Y = v.Y;

	dir.Normalize();
}


#pragma endregion
#pragma region Jump
// 점프
void AKMK_Player::InputJump(const struct FInputActionValue& value)
{
	movementComp->JumpZVelocity = jumpPower;
	Jump();
}
#pragma endregion
#pragma region Sit
// 앉기
void AKMK_Player::InputSit(const struct FInputActionValue& value)
{
	Crouch();
	springArms[1]->SetRelativeLocation(FVector(-70, 10, 0));
	springArms[0]->SetRelativeLocation(FVector(0, 0, 90));
}
// 일어나기
void AKMK_Player::InputStand(const struct FInputActionValue& value)
{
	UnCrouch();
	springArms[1]->SetRelativeLocation(FVector(-20, 0, 0));
	springArms[0]->SetRelativeLocation(FVector(0, 0, 140));
}

#pragma endregion
#pragma region Run
// 달리기
void AKMK_Player::InputRun(const struct FInputActionValue& value)
{
	movementComp->MaxWalkSpeed = speed * 2;
	springArm->SetRelativeLocation(FVector(-10, -5, 0));
}
// 걷기
void AKMK_Player::InputWalk(const struct FInputActionValue& value)
{
	movementComp->MaxWalkSpeed = speed;
	springArm->SetRelativeLocation(FVector(-20, 0, 0));
}

#pragma endregion
#pragma region interaction
// 상호작용 => 치트입니다
void AKMK_Player::InputE(const struct FInputActionValue& value)
{
	isIntarctive = true;
	// UGameplayStatics::OpenLevel(this, "JSH_Alpha");
	auto* load = CreateWidget(GetWorld(), LoadFact);
	load->AddToViewport(3);

	FTimerHandle timerHandle;
	GetWorldTimerManager().SetTimer(timerHandle, this, &AKMK_Player::ChangeLevel, loadTime, false);

}

void AKMK_Player::ChangeLevel()
{
	UGameplayStatics::OpenLevel(this, levelName);
}

void AKMK_Player::Cheat2(const struct FInputActionValue& value)
{
	isCheat2 = !isCheat2;
}
#pragma endregion
#pragma region ChangeHand
// 기본 손
void AKMK_Player::InputNum1(const struct FInputActionValue& value)
{
	UGameplayStatics::PlaySoundAtLocation(this, soundCue[7], GetActorLocation());
	isClick[0] = true;
	FSM->cnt = 0;
	anim->PlayChangeMontage();
	FSM->PState = PlayerHandFSM::Normal;
	whiteHand[0]->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
	isOn[0] = true;
}

// 총쏘기
void AKMK_Player::InputNum2(const struct FInputActionValue& value)
{
	UGameplayStatics::PlaySoundAtLocation(this, soundCue[7], GetActorLocation());
	if (FSM->isFire)
	{
		FSM->isFire = false;
		FSM->cnt = 0;
	}
	FSM->cnt = 0;
	isClick[1] = true;
	FSM->PState = PlayerHandFSM::GunPack;
	anim->PlayChangeMontage();
	whiteHand[1]->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
	isOn[1] = true;
}
// 점프 손
void AKMK_Player::InputNum3(const struct FInputActionValue& value)
{
	UGameplayStatics::PlaySoundAtLocation(this, soundCue[7], GetActorLocation());
	isClick[2] = true;
	FSM->cnt = 0;
	FSM->PState = PlayerHandFSM::JumpPack;
	anim->PlayChangeMontage();
	whiteHand[2]->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
	isOn[2] = true;
}



#pragma endregion
#pragma region MouseRight click
// 오른쪽 클릭시
void AKMK_Player::InputMR(const struct FInputActionValue& value)
{	
	playerRay->SetRayPos(startPos, endPos);
	Hands[0]->box->SetCollisionProfileName("Hand");
	isDir[0] = true;
}
void AKMK_Player::InputMRComp(const struct FInputActionValue& value)
{
	playerRay->isRay = false;
	isDir[0] = false;
}

#pragma endregion
#pragma region MouseLeft Click
// 왼쪽 클릭시
void AKMK_Player::InputML(const struct FInputActionValue& value)
{
	playerRay->SetRayPos(startPos, endPos);
	Hands[1]->box->SetCollisionProfileName("Hand");
	isDir[1] = true;
}
void AKMK_Player::InputMLComp(const struct FInputActionValue& value)
{
	playerRay->isRay = false;
	isDir[1] = false;
}
#pragma endregion

#pragma region overlapFunction
void AKMK_Player::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Other->GetActorLabel().Contains("Floor"))
	{
		FSM->PState = PlayerHandFSM::Normal;
	}
}

#pragma endregion
