#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "MeleeWeapon.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ECHOES_OF_DECAY_API AMeleeWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	AMeleeWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float AttackDamage;

	virtual void Attack() override;
};