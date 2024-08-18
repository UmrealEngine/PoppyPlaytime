#include "JSH_TopCatFSM.h"

#include "EngineUtils.h"
#include "JSH_Cat.h"
#include "JSH_CatDoor.h"
#include "JSH_CatFSM.h"
#include "JSH_Ending.h"
#include "JSH_Light.h"
#include "KHH_RotateDoor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UJSH_TopCatFSM::UJSH_TopCatFSM()
{
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UJSH_TopCatFSM::BeginPlay()
{
    Super::BeginPlay();

    

    // 상태 초기화
    tState = TCatState::Idle;

    // CatDoor 액터 가져오기
    CatDoor = Cast<AJSH_CatDoor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_CatDoor::StaticClass()));
    
  
    Cat = Cast<AJSH_Cat>(UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_Cat::StaticClass()));
    TArray<AActor*> FoundCats;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AJSH_Cat::StaticClass(), FoundCats);
    for (AActor* Actor : FoundCats)
    {
        if (Actor->Tags.Contains("FCat5"))
        {
            Cat = Cast<AJSH_Cat>(Actor);
            break;
        }
    }
    if (!Cat)
    {
        UE_LOG(LogTemp, Warning, TEXT("No AJSH_Cat with tag 'FCat5' found!"));
    }

    
    // 레벨의 모든 AJSH_Light 액터를 배열에 저장
    TArray<AActor*> FoundLights;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AJSH_Light::StaticClass(), FoundLights);

    for (AActor* Actor : FoundLights)
    {
        AJSH_Light* Light = Cast<AJSH_Light>(Actor);
        if (Light)
        {
            DoorLights.Add(Light);
        }
    }
    

    if (CatDoor)
    {
        // 'Door'라는 이름으로 액터의 참조를 저장합니다.
        FName DoorName = "Door";
        CatDoor->SetActorLabel(DoorName.ToString());
    }
    
    
    // PlayerHand = Cast<AKMK_PlayerHand>(UGameplayStatics::GetActorOfClass(GetWorld(), AKMK_PlayerHand::StaticClass()));
    //
    // if (!PlayerHand)
    // {
    //     UE_LOG(LogTemp, Warning, TEXT("KMK_PlayerHand not found in level!"));
    // }


    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("S5"), FoundActors);

    // 찾은 액터들 중에서 AJSH_Cat 타입의 액터를 CatS5에 저장합니다.
    for (AActor* Actor : FoundActors)
    {
        AJSH_Cat* FoundCat = Cast<AJSH_Cat>(Actor);
        if (FoundCat)
        {
            CatS5 = FoundCat;
            break;  // 첫 번째로 찾은 AJSH_Cat을 저장한 후 루프 종료
        }
    }


    EndingHelper = Cast<AJSH_Ending>(UGameplayStatics::GetActorOfClass(GetWorld(), AJSH_Ending::StaticClass()));
    
}

// Called every frame
void UJSH_TopCatFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    switch (tState)
    {
    case TCatState::Idle:
        IdleState(DeltaTime);
        break;

    case TCatState::Prepare:
        PrepareState(DeltaTime);
        break;
        
    case TCatState::TopOpen:
        TopOpenState(DeltaTime);
        break;
        
    case TCatState::Attack:
        AttackState();
        break;
    }

    // 화면에 상태 표시
    //FString myState = UEnum::GetValueAsString(tState);
    //DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), myState, nullptr, FColor::Blue, 0, true, 1);

    if (Cat->fsm->DoorOpen)
    {
        tState = TCatState::Prepare;
        currtime = 0;
        Cat->fsm->DoorOpen = false;
    }
}

void UJSH_TopCatFSM::IdleState(float DeltaTime)
{
    currtime += DeltaTime;

    // 모든 DoorLight를 꺼줍니다.
    for (AJSH_Light* Light : DoorLights)
    {
        if (Light)
        {
            Light->LightOn = false;
            Light->LightOff = true;
        }
    }

    // 일정 시간 후 태그 부여 , Tick에서 시간 초기화 해줌
    if (currtime >= addtag)
    {
        Cat->Tags.Add("FCat5");
    }
}

void UJSH_TopCatFSM::PrepareState(float DeltaTime)
{
    Cat->Tags.Empty();

    lighttime += DeltaTime;

    // 모든 DoorLight를 켭니다.
    for (AJSH_Light* Light : DoorLights)
    {
        if (Light)
        {
            Light->LightOn = true;
            Light->LightOff = false;
        }
    }

    if (lighttime >= opentime)
    {
        DoorOpen = false;
        topStart = true;
        lighttime = 0;
        tState = TCatState::TopOpen;
    }
}

void UJSH_TopCatFSM::TopOpenState(float DeltaTime)
{
    if (topStart)
    {
        static USoundWave* AttackSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Project/JSH/Audio/TopOpen.TopOpen'"));
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackSound, CatDoor->GetActorLocation());
        
        CatDoor->fsm->isOpen = true;
        CatDoor->fsm->cnt++;
        CatDoor->fsm->isComp = false;
        topStart = false;
    }

    if (CatDoor->GetActorRotation().Pitch >= 50)
    {
        currtime += DeltaTime;
        if (currtime >= toptime)
        {
            tState = TCatState::Attack;
            bHasAttacked = true;
            currtime = 0;
            start = false;
            //CatDoor->fsm->isOpen = false;
        }
    }
    
    if (CatDoor->GetActorRotation().Pitch >= 1)
    {
        start = true;
    }
    
    if (start)
    {
        if (CatDoor->GetActorRotation().Pitch <= 0)
        {
            tState = TCatState::Idle;
            //CatDoor->fsm->isOpen = false;
            currtime = 0;
            start = false;
        }  
    }
}

void UJSH_TopCatFSM::AttackState()
{

    //EndingHelper->JumpScareEnding = true;


    if (bHasAttacked)
    {
    	bHasAttacked = false;
        // Get me's location and store it in tt
    	if (TopCat)
    	{
    		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(TopCat);
    		
    		if (SpawnedActor)
    		{
    			// Set the actor to be hidden in the game
    			SpawnedActor->SetActorHiddenInGame(true);

    			// Optionally, disable collision if you don't want the actor to interact with other objects
    			SpawnedActor->SetActorEnableCollision(false);

    			// Get the location if needed
    			tt = SpawnedActor->GetActorLocation();
    			// Now you can use 'tt' as the location of the spawned actor

    		    CatS5->Destroy();
    		}
    	}
        
    	
        // Destroy me
        //me->fsm->BatCatStop();
        
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
}
