// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Projectile.h"
#include "EnemyRanged.generated.h"

UCLASS()
class ECHOES_OF_DECAY_API AEnemyRanged : public AEnemyBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AProjectile> ProjectileClass;

	virtual void AttackPlayer() override;
};
