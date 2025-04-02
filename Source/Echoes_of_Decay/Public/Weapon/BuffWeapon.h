#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "BuffWeapon.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ECHOES_OF_DECAY_API ABuffWeapon : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	ABuffWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float BuffDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float BuffMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XP")
	int32 XpGainAtUse = 2.0f;

	virtual void Attack() override;

	virtual void ApplyWeaponLevelEffects() override;
};
