#include "Weapon/RangedWeapon.h"
#include "GameFramework/Character.h"

ARangedWeapon::ARangedWeapon()
{
    WeaponType = EWeaponType::Ranged;
	WeaponName = "Bow";
    
}

void ARangedWeapon::BeginPlay()
{
    Super::BeginPlay();

    if (ProjectileClass)
    {
        AProjectile* DefaultProjectile = ProjectileClass->GetDefaultObject<AProjectile>();

        if (DefaultProjectile)
        {
            ProjectileDamage = DefaultProjectile->Damage;
        }
    }
}

void ARangedWeapon::Attack()
{
    if (!bCanAttack || !ProjectileClass || !Owner) return;


    FVector MuzzleLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100.0f;
    FRotator MuzzleRotation = Owner->GetActorRotation();

    AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation);
    if (!SpawnedProjectile) return;

	SpawnedProjectile->ProjectileOwner = this;
    GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &ARangedWeapon::ResetAttackCooldown, AttackCooldown, false);
}