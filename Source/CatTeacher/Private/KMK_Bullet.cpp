// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AKMK_Bullet::AKMK_Bullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
	SphereComp->SetSphereRadius(13.0f);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetRelativeScale3D(FVector(0.25f));

	// 충돌설정
	SphereComp->SetCollisionProfileName(TEXT("BlockAll"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 발사체 컴포넌트 설정
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->SetUpdatedComponent(RootComponent);

	MovementComp->InitialSpeed = 4000;
	MovementComp->MaxSpeed = 4000;
	//MovementComp->bShouldBounce = false;
}

// Called when the game starts or when spawned
void AKMK_Bullet::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [&]() {
		this->Destroy();
	}, 3.f, false);
}

// Called every frame
void AKMK_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKMK_Bullet::Die()
{
	Destroy();
}

//void AKMK_Bullet::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
//{
//	Destroy();
//}

