#include "Weapon/WeaponBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	Level = 1;
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
		XP -= XPToNextLevel;
		Level++;
		XPToNextLevel *= 1.5f;

		UE_LOG(LogTemp, Warning, TEXT("Weapon %s leveled up! Level: %d"), *WeaponName.ToString(), Level);
	}
}

void AWeaponBase::ResetAttackCooldown()
{
	bCanAttack = true;
	UE_LOG(LogTemp, Warning, TEXT("Attack cooldown reset!"));
}