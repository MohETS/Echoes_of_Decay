// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

// The AProjectile class handles a simple projectile's behavior in the game
UCLASS()
class ECHOES_OF_DECAY_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Mesh component for visual representation of the projectile
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* MeshComponent;

	// Movement component to handle projectile movement (speed, rotation, etc.)
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovement;

	// Damage the projectile inflicts when it hits another actor
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage = 20.0f;

	// Called when the projectile collides with something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
