#include "Enemies/EnemyMelee.h"
#include "Kismet/GameplayStatics.h"

void AEnemyMelee::AttackPlayer()
{
    if (!bCanAttack || !PlayerPawn) return;

    ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerPawn);
    if (!PlayerCharacter) return;

    EnemyState = EEnemyState::Attacking;
    UGameplayStatics::ApplyDamage(PlayerCharacter, Damage, GetController(), this, UDamageType::StaticClass());
    bCanAttack = false;
    GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyMelee::ResetAttackCooldown, AttackCooldown, false);
}