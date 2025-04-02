#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Projectile.h"
#include "RangedWeapon.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ECHOES_OF_DECAY_API ARangedWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	ARangedWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ProjectileDamage;

	virtual void Attack() override;
	virtual void ApplyWeaponLevelEffects() override;
};