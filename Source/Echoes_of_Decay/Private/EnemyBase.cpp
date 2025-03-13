// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense.h"
#include "Perception/AIPerceptionComponent.h"
#include "TimerManager.h"

AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;

	// Set up the AI perception component
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

    if (!SightConfig)
    {
        SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    }

	// Set up the sight configuration
    SightConfig->SightRadius = SightRadius;
    SightConfig->LoseSightRadius = SightRadius + 500.0f;
    SightConfig->PeripheralVisionAngleDegrees = 90.0f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// Configure the AI perception component
    AIPerceptionComponent->ConfigureSense(*SightConfig);

	// Set the dominant sense
    if (SightConfig->GetSenseImplementation())
    {
        AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Error: SightConfig->GetSenseImplementation() is NULL!"));
    }

	// Bind the perception function
    AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyBase::OnPerceptionUpdated);

	// Set up the movement component
    GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;
    GetCharacterMovement()->bOrientRotationToMovement = true;

	// Set up the AI controller
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    PatrolCenter = GetActorLocation();

    AAIController* AIController = Cast<AAIController>(GetController());
    if (!AIController)
    {
        UE_LOG(LogTemp, Error, TEXT("AIController is NULL ! Ennemi will not be able to move"));
    }

	// Start patrolling
	GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyBase::Patrol, 5.0f, true);
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (PlayerPawn)
    {
		// Check if the player is within the sight radius
        float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());

        if (Distance < SightRadius)
        {
            ChasePlayer();
        }
        if (Distance < AttackRange)
        {
            AttackPlayer();
        }
    }
}

void AEnemyBase::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (Actor && Actor == PlayerPawn)
    {
		// Check if the player was successfully sensed
        if (Stimulus.WasSuccessfullySensed())
        {
            UE_LOG(LogTemp, Warning, TEXT("Player detected!"));
            ChasePlayer();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Player lost!"));
            float DistanceToPatrolCenter = FVector::Dist(GetActorLocation(), PatrolCenter);

			// If the player is too far from the patrol zone, return to the patrol zone
            if (DistanceToPatrolCenter > PatrolMaxDistance)
            {
                UE_LOG(LogTemp, Warning, TEXT("Returning to patrol center"));
                AAIController* AIController = Cast<AAIController>(GetController());
                if (AIController)
                {
                    AIController->MoveToLocation(PatrolCenter);
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Resuming patrol..."));
                GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyBase::Patrol, 3.0f, true);
            }
        }
    }
}

void AEnemyBase::Patrol()
{
    UE_LOG(LogTemp, Warning, TEXT("Patrolling around patrol center"));

    if (AIPerceptionComponent->HasActiveStimulus(*PlayerPawn, UAISense::GetSenseID<UAISense_Sight>()))
    {
        UE_LOG(LogTemp, Warning, TEXT("Patrol stopped: player detected"));
        return;
    }

    FVector PatrolPoint = PatrolCenter + FMath::VRand() * PatrolMaxDistance / 2;
	PatrolPoint.Z = GetActorLocation().Z;

    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        UE_LOG(LogTemp, Warning, TEXT("Moving to patrol point: %s"), *PatrolPoint.ToString());
        AIController->MoveToLocation(PatrolPoint);
    }
}

void AEnemyBase::StartAttack()
{
	// Start the attack timer
    if (!GetWorldTimerManager().IsTimerActive(AttackTimerHandle))
    {
        GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyBase::AttackPlayer, 2.0f, true);
    }
}

void AEnemyBase::StopAttack()
{
	// Stop the attack timer
    GetWorldTimerManager().ClearTimer(AttackTimerHandle);
}

void AEnemyBase::ChasePlayer()
{
    if (PlayerPawn)
    {
		// Check if the player is within the sight radius
        float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
        float DistanceToPatrolCenter = FVector::Dist(GetActorLocation(), PatrolCenter);

		// If the player is too far from the patrol zone, return to the patrol zone
        AAIController* AIController = Cast<AAIController>(GetController());
        if (AIController)
        {
			// If the player is too far from the patrol zone, return to the patrol zone
            if (DistanceToPatrolCenter > PatrolMaxDistance)
            {
                UE_LOG(LogTemp, Warning, TEXT("Player too far from patrol zone, returning to patrol"));
                AIController->MoveToLocation(PatrolCenter);
                return;
            }

			// If the player is within the attack range, stop moving
            if (DistanceToPlayer > AttackRange)
            {
                UE_LOG(LogTemp, Warning, TEXT("Chasing player..."));
                AIController->MoveToActor(PlayerPawn, AttackRange - 100.0f);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Stopping movement - player within attack range"));
                AIController->StopMovement();
            }
        }
    }
}

void AEnemyBase::AttackPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Attacking! Base class"));
}