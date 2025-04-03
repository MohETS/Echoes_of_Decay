#include "Weapon/WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	XP = 0;
	XPToNextLevel = 100;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack AWeaponBase called!"));
}

void AWeaponBase::GainXP(int32 Amount)
{
	XP += Amount;

	if (XP >= XPToNextLevel)
	{
		LevelUp();
		UE_LOG(LogTemp, Warning, TEXT("Weapon %s leveled up! Level: %d"), *WeaponName.ToString(), WeaponLevel);
	}
}

void AWeaponBase::ResetAttackCooldown()
{
	bCanAttack = true;
	UE_LOG(LogTemp, Warning, TEXT("Attack cooldown reset!"));
}

void AWeaponBase::LevelUp()
{
	if (WeaponLevel < MaxWeaponLevel)
	{
		WeaponLevel++;
		XPToNextLevel *= 1.5f;
		ApplyWeaponLevelEffects();
		UE_LOG(LogTemp, Log, TEXT("Weapon leveled up to %d!"), WeaponLevel);
	}
}

void AWeaponBase::ApplyWeaponLevelEffects()
{
}
