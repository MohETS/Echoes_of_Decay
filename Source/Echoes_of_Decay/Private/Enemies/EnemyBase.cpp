#include "Enemies/EnemyBase.h"
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
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Enemies/EnemyHealthBar.h"
#include "MyCharacter.h"
#include "Engine/OverlapResult.h"

// Sets default values
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
    GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;
    GetCharacterMovement()->bOrientRotationToMovement = true;

    HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
    HealthBarWidget->SetupAttachment(RootComponent);
    HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
    HealthBarWidget->SetDrawSize(FVector2D(150.f, 20.f));
    HealthBarWidget->SetWidgetClass(HealthBarWidgetClass);

    // Set up the AI controller
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
    Super::BeginPlay();

    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    PatrolCenter = GetActorLocation();

    AIController = Cast<AAIController>(GetController());
    if (!AIController)
    {
        UE_LOG(LogTemp, Error, TEXT("AIController is NULL ! Ennemi will not be able to move"));
    }
    // Start patrolling
    GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyBase::Patrol, PatrolingTime, true);
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!PlayerPawn) return;

    float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());

    if (Distance < SightRadius)
    {
        ChasePlayer();
    }

    if (Distance < AttackRange)
    {
        AttackPlayer();
    }

    CheckForNearbyPlayer();
}

void AEnemyBase::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (!Actor || Actor != PlayerPawn) return;

    // Check if the player was successfully sensed
    if (Stimulus.WasSuccessfullySensed())
    {
        UE_LOG(LogTemp, Warning, TEXT("Player detected!"));
        ChasePlayer();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Player lost!"));
        AIController->ClearFocus(EAIFocusPriority::Gameplay);
        float DistanceToPatrolCenter = FVector::Dist(GetActorLocation(), PatrolCenter);

        // If the player is too far from the patrol zone, return to the patrol zone
        if (DistanceToPatrolCenter > PatrolMaxDistance)
        {
            UE_LOG(LogTemp, Warning, TEXT("Returning to patrol center"));
            if (!AIController) return;
            AIController->MoveToLocation(PatrolCenter);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Resuming patrol..."));
            GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyBase::Patrol, 1.0f, true);
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

    GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
    FVector PatrolPoint = PatrolCenter + FMath::VRand() * PatrolMaxDistance / 2;
    PatrolPoint.Z = GetActorLocation().Z;

    if (!AIController) return;

    EnemyState = EEnemyState::Patrolling;
    UE_LOG(LogTemp, Warning, TEXT("Moving to patrol point: %s"), *PatrolPoint.ToString());
    AIController->MoveToLocation(PatrolPoint);
}

void AEnemyBase::ChasePlayer()
{
    if (!PlayerPawn) return;
    AIController->SetFocus(PlayerPawn);
    GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
    EnemyState = EEnemyState::Chasing;
    // Check if the player is within the sight radius
    float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
    float DistanceToPatrolCenter = FVector::Dist(GetActorLocation(), PatrolCenter);

    // If the player is too far from the patrol zone, return to the patrol zone
    if (!AIController) return;

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

void AEnemyBase::AttackPlayer()
{
    UE_LOG(LogTemp, Warning, TEXT("Enemy Attacking! Base class"));
}

// Handle the reception of damage
float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    Health -= ActualDamage;
    UE_LOG(LogTemp, Warning, TEXT("Enemy received %f damage. Health remaining: %f"), ActualDamage, Health);
    GetWorldTimerManager().ClearTimer(RegenTickTimer);
    GetWorldTimerManager().ClearTimer(RegenStartTimer);

    GetWorldTimerManager().SetTimer(RegenStartTimer, this, &AEnemyBase::StartHealthRegen, TimeBeforeRegenStarts, false);
    UpdateHealthBar();
    if (Health <= 0.0f) Die(DamageCauser);
    return ActualDamage;
}

void AEnemyBase::StartHealthRegen()
{
    if (Health >= MaxHealth || bIsPlayerNearby) return;

    GetWorldTimerManager().SetTimer(RegenTickTimer, this, &AEnemyBase::RegenHealth, RegenInterval, true);
}

void AEnemyBase::RegenHealth()
{
    Health = FMath::Clamp(Health + RegenAmount, 0.0f, MaxHealth);
    UpdateHealthBar();

    if (Health >= MaxHealth)
    {
        GetWorldTimerManager().ClearTimer(RegenTickTimer);
    }
}

void AEnemyBase::UpdateHealthBar()
{
    if (HealthBarWidget)
    {
        UEnemyHealthBar* HealthBar = Cast<UEnemyHealthBar>(HealthBarWidget->GetUserWidgetObject());
        if (HealthBar)
        {
            HealthBar->UpdateHealthBar(Health, MaxHealth);
        }
    }
}

void AEnemyBase::CheckForNearbyPlayer()
{
    bIsPlayerNearby = false;

    FVector PlayerLocation = GetActorLocation();
    TArray<FOverlapResult> Overlaps;

    FCollisionShape Sphere = FCollisionShape::MakeSphere(EnemyProximityRadius);

    bool bHit = GetWorld()->OverlapMultiByObjectType(
        Overlaps,
        PlayerLocation,
        FQuat::Identity,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
        Sphere
    );

    if (bHit)
    {
        for (auto& Result : Overlaps)
        {
            AActor* OtherActor = Result.GetActor();
            if (OtherActor && OtherActor != this && OtherActor->IsA<AMyCharacter>())
            {
                bIsPlayerNearby = true;
                GetWorldTimerManager().ClearTimer(RegenTickTimer);
                GetWorldTimerManager().ClearTimer(RegenStartTimer);
                GetWorldTimerManager().SetTimer(RegenStartTimer, this, &AEnemyBase::StartHealthRegen, TimeBeforeRegenStarts, false);
                break;
            }
        }
    }
}

void AEnemyBase::Die(AActor* Killer)
{
    UE_LOG(LogTemp, Warning, TEXT("Enemy has died!"));
    EnemyState = EEnemyState::Dying;
    if (Cast<AMyCharacter>(Killer))
    {
        Cast<AMyCharacter>(Killer)->GainWeaponXP(XpAtDeath);
    }
}

void AEnemyBase::ResetAttackCooldown()
{
    bCanAttack = true;
    EnemyState = EEnemyState::Idle;
    UE_LOG(LogTemp, Warning, TEXT("Attack cooldown reset!"));
}