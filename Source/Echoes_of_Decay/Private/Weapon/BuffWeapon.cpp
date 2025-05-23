#include "Weapon/BuffWeapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

ABuffWeapon::ABuffWeapon()
{
	WeaponType = EWeaponType::Buff;
	WeaponName = "Magic Wand";
	BuffDuration = 5.0f;
	BuffMultiplier = 1.1f;
}

void ABuffWeapon::Attack()
{
    UE_LOG(LogTemp, Warning, TEXT("%s casts a buff!"), *WeaponName.ToString());

    if (WeaponEffect)
    {
        WeaponEffect->ApplyEffect(Owner, Owner);
        GainXP(XpGainAtUse);
    }
}

void ABuffWeapon::ApplyWeaponLevelEffects()
{
    BuffDuration *= 1.1f;
    BuffMultiplier *= 1.1f;
}