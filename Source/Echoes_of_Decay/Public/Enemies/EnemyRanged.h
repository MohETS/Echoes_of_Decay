#pragma once

#include "CoreMinimal.h"
#include "Enemies/EnemyBase.h"
#include "Projectile.h"
#include "EnemyRanged.generated.h"

UCLASS()
class ECHOES_OF_DECAY_API AEnemyRanged : public AEnemyBase
{
	GENERATED_BODY()
	
protected:
	// Classe du projectile à tirer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	virtual void AttackPlayer() override;
};
