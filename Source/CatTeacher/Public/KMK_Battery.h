#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KMK_Battery.generated.h"

UCLASS()
class CATTEACHER_API AKMK_Battery : public AActor
{
    GENERATED_BODY()

public:    
    // Sets default values for this actor's properties
    AKMK_Battery();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UStaticMeshComponent* meshComp = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UBoxComponent* box;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool isPush = false;
    UPROPERTY()
    class AKMK_PlayerHand* hand;
    UPROPERTY()
    TArray<AActor*> actors;
    UPROPERTY()
    class UKMK_Bat* bat;

    UFUNCTION()
    void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    void SetVis(bool isVis);

};
