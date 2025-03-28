#include "Weapon/RangedWeapon.h"
#include "GameFramework/Character.h"

ARangedWeapon::ARangedWeapon()
{
    WeaponType = EWeaponType::Ranged;
	WeaponName = "Bow";
	ProjectileDamage = 15.0f;
}

void ARangedWeapon::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Ranged Weapon %s used attack!"), *WeaponName.ToString());
    if (ProjectileClass)
    {
        FVector MuzzleLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100.0f;
        FRotator MuzzleRotation = Owner->GetActorRotation();

        AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation);
        if (SpawnedProjectile)
        {
			SpawnedProjectile->ProjectileOwner = this;
            UE_LOG(LogTemp, Warning, TEXT("Projectile spawned successfully!"));
        }
    }
}