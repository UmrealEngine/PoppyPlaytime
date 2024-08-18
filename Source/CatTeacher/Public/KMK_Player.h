// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KMK_Player.generated.h"

UCLASS()
class CATTEACHER_API AKMK_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKMK_Player();
#pragma region CreateVar

	UPROPERTY(VisibleAnywhere, Category = "Camera", BlueprintReadWrite)
	class USpringArmComponent* springArm = nullptr;
	UPROPERTY(EditAnywhere, Category = "Camera", BlueprintReadWrite)
	TArray<class USpringArmComponent*> springArms;

	UPROPERTY(EditAnywhere, Category = "Camera", BlueprintReadWrite)
	class UCameraComponent* camera = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Component")
	class UKMK_PlayerHandFSM* FSM = nullptr;

	UPROPERTY(EditAnywhere, Category = "Modeling", BlueprintReadWrite)
	class USkeletalMeshComponent* armMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Ray")
	class UKMK_PlayerRay* playerRay = nullptr;

	UPROPERTY(EditAnywhere, Category = "Hand")
	TArray<class UCableComponent*> CableComp;
	UPROPERTY(EditAnywhere, Category = "Hand")
	TArray<class USceneComponent*> SceneComp;

	// Player에게 pawn sensor 만들기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPawnSensingComponent* sensor;

#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
	class UCharacterMovementComponent* movementComp = nullptr;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region HandActorFact
	UPROPERTY(EditAnywhere, Category = "Hand")
	TSubclassOf<class AKMK_PlayerHand> LHandFact;
	UPROPERTY(EditAnywhere, Category = "Hand")
	TSubclassOf<class AKMK_PlayerHand> RHandFact;
	UPROPERTY(EditAnywhere, Category = "Hand")
	TSubclassOf<class AKMK_Battery> batteryFact;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class AKMK_PlayerHand*> Hands;

	UPROPERTY()
	TArray<class AKMK_Battery*> Bats;
	UPROPERTY()
	class UStaticMeshComponent* RMeshComp;
#pragma endregion
#pragma region InputVaribles
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_Mapping = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Turn = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Jump = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Sit = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Run = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_ClickE = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Click1 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Click2 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Click3 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_ClickR = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_ClickL = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Cheat1 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* IA_Cheat2 = nullptr;
#pragma endregion
#pragma region variables
	FVector dir = FVector(0);

	UPROPERTY(EditAnywhere, Category = "Variables")
	float speed = 600.f;
	UPROPERTY()
	float jumpPower = 0;

	UPROPERTY()
	TArray<bool> isDir = {false, false};
	UPROPERTY()
	TArray<bool> isClick = {false, false, false};

	UPROPERTY()
	bool isIntarctive = false;
#pragma endregion
#pragma region Function
	// ȸ��ó��
	void InputTurn(const struct FInputActionValue& value);
	// �¿� �̵�
	void InputMove(const struct FInputActionValue& value);
	// ����
	void InputJump(const struct FInputActionValue& value);
	// �ɱ�
	void InputSit(const struct FInputActionValue& value);
	void InputStand(const struct FInputActionValue& value);
	// �޸���
	void InputRun(const struct FInputActionValue& value);
	void InputWalk(const struct FInputActionValue& value);
	// ��ȣ�ۿ�
	void InputE(const struct FInputActionValue& value);
	// ���� �ٲٱ�
	void InputNum1(const struct FInputActionValue& value);
	void InputNum2(const struct FInputActionValue& value);
	void InputNum3(const struct FInputActionValue& value);

	//void Cheat1(const struct FInputActionValue& value);
	//bool isCheat1 = false;
	void Cheat2(const struct FInputActionValue& value);
	UPROPERTY(BlueprintReadWrite)
	bool isCheat2 = false;
	// ���콺 Ŭ�� 
	// 1. ongoing�϶�, �����Ÿ� ���� ��ü�� ������ ��ü�� ��� ���·� ����
	// 2. cancle �Ǹ�, ray �������� �����
	// 3. ���� �����϶�, ĵ���Ǹ� ray���
	// ������
	void InputMR(const struct FInputActionValue& value);
	void InputMRComp(const struct FInputActionValue& value);
	// �޼�
	void InputML(const struct FInputActionValue& value);
	void InputMLComp(const struct FInputActionValue& value);

#pragma endregion
#pragma region ray variables
	FVector startPos;
	FVector endPos;
	FVector endPos1;
	UPROPERTY(EditAnywhere)
	float rayDis = 1000;
	UPROPERTY(EditAnywhere)
	float rayDis1 = 2000;
#pragma endregion
#pragma region Overlap
	UFUNCTION()
	void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);
	// 오버랩 감지를 위한 함수 선언
	/*virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);*/
#pragma endregion

	UPROPERTY()
	class UPlayerAnimInstance* anim;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WidFact;
	UPROPERTY()
	class UPlayerWidget* widget;	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoadFact;

	UPROPERTY(EditAnywhere)
    class UMaterialInstance* matFact;
	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* myMatDynamic;
	float gauzeSpd = 1;

	UPROPERTY()
	class UImage* image;

	bool bChangeHand;
	bool bFireBullet;

	FORCEINLINE void ChangeHandEnd() {bChangeHand = true;}
	FORCEINLINE void FireBulletEnd() { bFireBullet = true;}

	void ChangeLevel();


	UPROPERTY(EditAnywhere)
	FName levelName = "JSH_Alpha1";
	UPROPERTY(EditAnywhere)
	float loadTime = 3;
	UPROPERTY(EditAnywhere)
	float disSpeed = 1;

	UPROPERTY()
	TArray<class UImage*> whiteHand;

	TArray<bool>isOn = { false, false, false};
	float disTime = 1;

	UPROPERTY(EditAnywhere, Category = "Hand")
	UStaticMesh* HandMesh;

	// Sound 관련
	UPROPERTY(VisibleAnywhere)
	TArray<class UAudioComponent*> audioComps;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TArray<class USoundCue*> soundCue;
	UPROPERTY()
	TArray<bool> isSFX = {false, false, false};
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float walkSpeed = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float RunSpeed = 1;

	void PlayPlayerSound(int index);
    void StopPlayerSound(int index);

	float GetVeloFunc();

	virtual void Landed(const FHitResult& Hit) override;
};
