#include "JSH_CatFSM.h"

#include "EngineUtils.h"
#include "JSH_Cat.h"
#include "KMK_Player.h"
#include "Kismet/GameplayStatics.h"
#include "JSH_Target.h"
#include "MovieSceneSequenceID.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JSH_Battery.h"
#include "JSH_CatDoor.h"
#include "JSH_Ending.h"
#include "JSH_Random.h"
#include "KHH_RotateDoor.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraActor.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"

// Sets default values for this component's properties
UJSH_CatFSM::UJSH_CatFSM()
{
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UJSH_CatFSM::BeginPlay()
{
    Super::BeginPlay();
    
    me = Cast<AJSH_Cat>(GetOwner());

    //cState = ECatState::RoundMove;
    cState = ECatState::RoundMove;

    
    
    // CatNab 초기 이동 속도 조절
    if (me && me->GetCharacterMovement())
    {
        me->GetCharacterMovement()->MaxWalkSpeed = 200.0f; // Set the desired max speed
    }


    // Cat Door 찾기
    CatDoor = Cast<AJSH_CatDoor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_CatDoor::StaticClass()));

    if (CatDoor)
    {
        // 'Door'라는 이름으로 액터의 참조를 저장합니다.
        FName DoorName = "Door";
        CatDoor->SetActorLabel(DoorName.ToString());
    }

    DoorOpen = false;



    EndingHelper = Cast<AJSH_Ending>(UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_Ending::StaticClass()));
}

void UJSH_CatFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    switch (cState)
    {
    case ECatState::Idle:
        IdleState(DeltaTime);
        break;
        
    case ECatState::RoundMove:
        RoundMoveState();
        break;
        
    case ECatState::MoveWait:
        MoveWaitState();
        break;
        
    case ECatState::TrueMove:
        TrueMoveState();
        break;
                
    case ECatState::FalseMoveWait:
        FalseMoveWaitState();
        break;

    case ECatState::FalseMove:
        FalseMoveState();
        break;

    case ECatState::Ceiling:
        CeilingState();
        break;
        
    case ECatState::Discovery:
        DiscoveryState();
        break;
        
    case ECatState::Attack:
        AttackState();
        break;

    case ECatState::top:
        TopState();
        break;
        
    case ECatState::Die:
        DieState();
        break;

    case ECatState::ending:
        EndingState();
        break;
    }

    //FString logMsg = UEnum::GetValueAsString(cState);
    //GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMsg);

    /*FString logMsg = UEnum::GetValueAsString(cState);
    logMsg += TEXT(" - SelectedTag: ") + SelectedTag.ToString();
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, logMsg);*/


    /*FString logMsg2 = UEnum::GetValueAsString(cState);
    logMsg += TEXT(" - SelectedTag: ") + SelectedTag.ToString();
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, logMsg2);*/

    // CatNab 상태 표시
    //FString myState = UEnum::GetValueAsString(cState);
    //DrawDebugString(GetWorld() , GetOwner()->GetActorLocation(), myState , nullptr , FColor::Yellow , 0, true, 1);

    UpdateState();
    UpdateStateFalse();

    if (SwSt1)
    {
        if (me && me->Tags.Contains("S1"))
        {
            SwSt1 = false;
            IdleTeleport = true;
            cState = ECatState::Idle;
        }
    }
    else if (SwSt2)
    {
        if (me && me->Tags.Contains("S2"))
        {
            SwSt2 = false;
            IdleTeleport = true;
            cState = ECatState::Idle;
        } 
    }
    else if (SwSt3)
    {
        if (me && me->Tags.Contains("S3"))
        {
            SwSt3 = false;
            IdleTeleport = true;
            cState = ECatState::Idle;
        } 
    }
    else if (SwSt4)
    {
        if (me && me->Tags.Contains("S4"))
        {
            SwSt4 = false;
            IdleTeleport = true;
            cState = ECatState::Idle;
        } 
    }

    // if (DoorOpen)
    // {
    //     cState = ECatState::top;
    //     DoorOpen = false;
    //     topStart = true;
    // }
}

void UJSH_CatFSM::UpdateState()
{
    if (SelectedTag == "FCat1")
    {
        if (me && me->Tags.Contains("FCat1"))
        {
            cState = ECatState::MoveWait;
            me->Tags.Remove("FCat1");
            SelectedTag = "nullptr";
        }
    }
    else if (SelectedTag == "FCat2")
    {
        if (me && me->Tags.Contains("FCat2"))
        {
            cState = ECatState::MoveWait;
            me->Tags.Remove("FCat2");
            SelectedTag = "nullptr";
        }
    }
    else if (SelectedTag == "FCat3")
    {
        if (me && me->Tags.Contains("FCat3"))
        {
            cState = ECatState::MoveWait;
            me->Tags.Remove("FCat3");
            SelectedTag = "nullptr";
        }
    }
    else if (SelectedTag == "FCat4")
    {
        if (me && me->Tags.Contains("FCat4"))
        {
            cState = ECatState::MoveWait;
            me->Tags.Remove("FCat4");
            SelectedTag = "nullptr";
        }
    }
}

// 환상
void UJSH_CatFSM::UpdateStateFalse()
{
    if (SelectedTagFalse == "FCat1")
    {
        if (me && me->Tags.Contains("FCat1"))
        {
            cState = ECatState::FalseMoveWait;
            me->Tags.Remove("FCat1");
            SelectedTagFalse = "nullptr";
        }
    }
    else if (SelectedTagFalse == "FCat2")
    {
        if (me && me->Tags.Contains("FCat2"))
        {
            cState = ECatState::FalseMoveWait;
            me->Tags.Remove("FCat2");
            SelectedTagFalse = "nullptr";
        }
    }
    else if (SelectedTagFalse == "FCat3")
    {
        if (me && me->Tags.Contains("FCat3"))
        {
            cState = ECatState::FalseMoveWait;
            me->Tags.Remove("FCat3");
            SelectedTagFalse = "nullptr";
        }
    }
    else if (SelectedTagFalse == "FCat4")
    {
        if (me && me->Tags.Contains("FCat4"))
        {
            cState = ECatState::FalseMoveWait;
            me->Tags.Remove("FCat4");
            SelectedTagFalse = "nullptr";
        }
    }
}

void UJSH_CatFSM::BatCatStop()
{
    me->FalseBox->SetCollisionProfileName(TEXT("NoCollision"));
    FVector TargetLocation = target01->GetActorLocation();
    me->SetActorLocation(TargetLocation);
    
    cState = ECatState::Die;
}

void UJSH_CatFSM::batoff()
{
    cState = ECatState::Idle;
}




void UJSH_CatFSM::IdleState(float DeltaTime)
{
    me->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
    currentTime += DeltaTime;
    
    if (IdleTeleport)
    {
        me->FalseBox->SetCollisionProfileName(TEXT("NoCollision"));
        FVector TargetLocation = target01->GetActorLocation();
        me->SetActorLocation(TargetLocation);
        IdleTeleport = false;
    }

    if (currentTime >= idletime)
    {
        if (me && me->Tags.Contains("S1"))
        {
            me->Tags.Add("FCat1");
        }
        else if (me && me->Tags.Contains("S2"))
        {
            me->Tags.Add("FCat2");
        }
        else if (me && me->Tags.Contains("S3"))
        {
            me->Tags.Add("FCat3");
        }
        else if (me && me->Tags.Contains("S4"))
        {
            me->Tags.Add("FCat4");
        }
        
        GEngine->AddOnScreenDebugMessage(8, 1, FColor::Blue, FString::Printf(TEXT("change")));
        cState = ECatState::RoundMove;
        currentTime = 0;
    }
}

void UJSH_CatFSM::RoundMoveState()
{
    // 왕복 이동 속도
    me->GetCharacterMovement()->MaxWalkSpeed = RoundMoveSpeed;
    
    static bool bMovingToTarget01_FCat1 = true; // FCat1 대상에 대한 이동 플래그
    static bool bMovingToTarget01_FCat2 = true; // FCat2 대상에 대한 이동 플래그
    static bool bMovingToTarget01_FCat3 = true; // FCat3 대상에 대한 이동 플래그
    static bool bMovingToTarget01_FCat4 = true; // FCat3 대상에 대한 이동 플래그

    TArray<AActor*> TaggedActors_FCat1;
    TArray<AActor*> TaggedActors_FCat2;
    TArray<AActor*> TaggedActors_FCat3;
    TArray<AActor*> TaggedActors_FCat4;

    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat1"), TaggedActors_FCat1);
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat2"), TaggedActors_FCat2);
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat3"), TaggedActors_FCat3);
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FCat4"), TaggedActors_FCat4);

    // me가 FCat1 태그를 가지고 있는지 확인
    bool bIsTagged_FCat1 = TaggedActors_FCat1.Contains(me);

    // me가 FCat2 태그를 가지고 있는지 확인
    bool bIsTagged_FCat2 = TaggedActors_FCat2.Contains(me);

    // me가 FCat3 태그를 가지고 있는지 확인
    bool bIsTagged_FCat3 = TaggedActors_FCat3.Contains(me);

    // me가 FCat4 태그를 가지고 있는지 확인
    bool bIsTagged_FCat4 = TaggedActors_FCat4.Contains(me);

    if (bIsTagged_FCat1)
    {
        // me가 FCat1 태그를 가진 경우에만 아래 코드를 실행
        FVector destination;
        if (bMovingToTarget01_FCat1)
        {
            destination = target01->GetActorLocation();
        }
        else
        {
            destination = target02->GetActorLocation();
        }

        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        float distance = FVector::Dist(me->GetActorLocation(), destination);
        if (distance < ReachDistance)
        {
            // 목표 지점 도착, 이동 목표를 전환
            bMovingToTarget01_FCat1 = !bMovingToTarget01_FCat1;
        }
    }
    else if (bIsTagged_FCat2)
    {
        // me가 FCat2 태그를 가진 경우에만 아래 코드를 실행
        FVector destination;
        if (bMovingToTarget01_FCat2)
        {
            destination = target01->GetActorLocation();
        }
        else
        {
            destination = target02->GetActorLocation();
        }

        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        float distance = FVector::Dist(me->GetActorLocation(), destination);
        if (distance < ReachDistance)
        {
            // 목표 지점 도착, 이동 목표를 전환
            bMovingToTarget01_FCat2 = !bMovingToTarget01_FCat2;
        }
    }
    else if (bIsTagged_FCat3)
    {
        // me가 FCat3 태그를 가진 경우에만 아래 코드를 실행
        FVector destination;
        if (bMovingToTarget01_FCat3)
        {
            destination = target01->GetActorLocation();
        }
        else
        {
            destination = target02->GetActorLocation();
        }

        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        float distance = FVector::Dist(me->GetActorLocation(), destination);
        if (distance < ReachDistance)
        {
            // 목표 지점 도착, 이동 목표를 전환
            bMovingToTarget01_FCat3 = !bMovingToTarget01_FCat3;
        }
    }
    else if (bIsTagged_FCat4)
    {
        // me가 FCat3 태그를 가진 경우에만 아래 코드를 실행
        FVector destination;
        if (bMovingToTarget01_FCat4)
        {
            destination = target01->GetActorLocation();
        }
        else
        {
            destination = target02->GetActorLocation();
        }

        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        float distance = FVector::Dist(me->GetActorLocation(), destination);
        if (distance < ReachDistance)
        {
            // 목표 지점 도착, 이동 목표를 전환
            bMovingToTarget01_FCat4 = !bMovingToTarget01_FCat4;
        }
    }
}

void UJSH_CatFSM::MoveWaitState()
{
    // 위치 가운데 정렬 
    FVector destinationM = targetMiddle->GetActorLocation();
    FVector dirM = destinationM - me->GetActorLocation();
    me->AddMovementInput(dirM.GetSafeNormal());

    if (dirM.Size() < ReachDistance)
    {
        cState = ECatState::TrueMove;
    }
}

void UJSH_CatFSM::TrueMoveState()
{
    me->GetCharacterMovement()->MaxWalkSpeed = AttackMoveSpeed;

    FVector destination = targetPlayer->GetActorLocation();
    FVector dir = destination - me->GetActorLocation();
    me->AddMovementInput(dir.GetSafeNormal());

    if (dir.Size() < ReachDistance)
    {
        bHasAttacked = true;
        cState = ECatState::Attack;
    }
}

void UJSH_CatFSM::FalseMoveWaitState()
{
    // 위치 가운데 정렬 
    FVector destinationM = targetMiddle->GetActorLocation();
    FVector dirM = destinationM - me->GetActorLocation();
    me->AddMovementInput(dirM.GetSafeNormal());

    if (dirM.Size() < ReachDistance)
    {
        cState = ECatState::FalseMove;
    }
}

void UJSH_CatFSM::FalseMoveState()
{
    // 가짜 고양이 총으로 삭제 위해서 Collision 변경
    me->FalseBox->SetCollisionProfileName(TEXT("FalseCat"));
    
    me->GetCharacterMovement()->MaxWalkSpeed = AttackMoveSpeed;
    
    FVector destination = targetPlayer->GetActorLocation();
    FVector dir = destination - me->GetActorLocation();
    me->AddMovementInput(dir.GetSafeNormal());

    if (dir.Size() < ReachDistance)
    {
        bHasAttacked = true;
        cState = ECatState::Attack;
    }
}

void UJSH_CatFSM::CeilingState()
{
}

void UJSH_CatFSM::DiscoveryState()
{
}


void UJSH_CatFSM::AttackState()
{
    me->FalseBox->SetCollisionProfileName(TEXT("NoCollision"));

    //EndingHelper->JumpScareEnding = true;

    // if(ccamera)
    // {
    //     GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(ccamera);
    // }
    
    if (bHasAttacked)
    {
        // Get me's location and store it in tt
        tt = me->GetActorLocation();

        // Destroy me
        me->fsm->BatCatStop();
        
        // Get the player controller
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            // Get the player's pawn (character or any actor controlled by the player)
            APawn* PlayerPawn = PlayerController->GetPawn();
            if (PlayerPawn)
            {
                // Calculate the direction vector from the player to tt
                FVector Direction = tt - PlayerPawn->GetActorLocation();
                Direction.Z = 0; // Keep the camera level, ignore the height difference
                
                // Calculate the new rotation for the camera to look at tt
                FRotator NewControlRotation = Direction.Rotation();
                
                // Adjust the Pitch to make the camera look slightly upwards (e.g., +15 degrees)
                NewControlRotation.Pitch += 23.0f;

                // Set the player's control rotation to the new rotation
                PlayerController->SetControlRotation(NewControlRotation);
 
                // Disable the player's ability to control the camera
                PlayerController->SetIgnoreLookInput(true);

                // Disable the player's ability to move
                PlayerController->SetIgnoreMoveInput(true);

                // Find all actors with the tag "FcatAttack"
                TArray<AActor*> FoundActors;
                UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("FcatAttack"), FoundActors);

                for (AActor* Actor : FoundActors)
                {
                    // Calculate the position 1 cm in front of the player's eyes
                    FVector EyeLocation;
                    FRotator EyeRotation;
                    PlayerPawn->GetActorEyesViewPoint(EyeLocation, EyeRotation);
                    FVector NewLocation = EyeLocation + EyeRotation.Vector() * 1.0f;

                    // Set the actor's location
                    Actor->SetActorLocation(NewLocation);

                    // Calculate the new rotation to look in the opposite direction of the player's view
                    FRotator OppositeRotation = EyeRotation;
                    OppositeRotation.Yaw += 180.0f; // Rotate 180 degrees around the Z axis (Yaw)

                    // Set the actor's rotation
                    Actor->SetActorRotation(OppositeRotation);

                    AJSH_Cat* atcat = Cast<AJSH_Cat>(Actor);
                    
                    atcat->PlayAttackAnimation = true;  // --->> cat tick
                }
            }
            //me->PlayAttackAnimation = true;
        }
    }

    // if (AttackCatDestroy)
    // {
    //     me->Destroy();
    //
    //     if (me && me->Tags.Contains("S1"))
    //     {
    //         me->Destroy();
    //     }
    //     if (me && me->Tags.Contains("S2"))
    //     {
    //         me->Destroy();
    //     }
    //     if (me && me->Tags.Contains("S3"))
    //     {
    //         me->Destroy();
    //     }
    //     if (me && me->Tags.Contains("S4"))
    //     {
    //         me->Destroy();
    //     }
    //
    //     Random->Stop = true;
    //     AttackCatDestroy = false;
    // }
}



void UJSH_CatFSM::TopState()
{
    // if (topStart)
    // {
    //     CatDoor->fsm->isOpen = true;
    //     topStart = false;
    // }
}

void UJSH_CatFSM::DieState()
{
    if (me && me->Tags.Contains("S3"))
    {
        me->Tags.Remove("FCat3");
    }
    

    if (me && me->Tags.Contains("S4"))
    {
        me->Tags.Remove("FCat4");
    }
}


void UJSH_CatFSM::EndingState()
{
    if (ending)
    {
        //FVector TargetLocation = target01->GetActorLocation();
        //me->SetActorLocation(TargetLocation);
        me->Destroy();

        if (me && me->Tags.Contains("S1"))
        {
            me->Destroy();
        }
        if (me && me->Tags.Contains("S2"))
        {
            me->Destroy();
        }
        if (me && me->Tags.Contains("S3"))
        {
            me->Destroy();
        }
        if (me && me->Tags.Contains("S4"))
        {
            me->Destroy();
        }
        
        ending = false;
    }
}
