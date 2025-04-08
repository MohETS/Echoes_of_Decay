#include "Enemies/EnemyRanged.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

void AEnemyRanged::AttackPlayer()
{
    UE_LOG(LogTemp, Warning, TEXT("Ranged Enemy Shooting!"));

    if (!bCanAttack || !ProjectileClass || !PlayerPawn) return;
    
    FVector ToPlayer = (PlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FVector Forward = GetActorForwardVector();

    float DotProduct = FVector::DotProduct(Forward, ToPlayer);
    float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

    float MaxAngle = 75.0f;
    if (AngleDegrees > MaxAngle) return;

    AIController->SetFocus(PlayerPawn);
    bCanAttack = false;
    EnemyState = EEnemyState::Attacking;
    FVector SpawnLocation = GetActorLocation() + Forward * 100.0f;
    FRotator SpawnRotation = ToPlayer.Rotation();
    GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
    GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyRanged::ResetAttackCooldown, AttackCooldown, false);
}