#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
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

	// Movement component to handle projectile movement (speed, rotation, etc.)
	UPROPERTY(EditAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Components")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float LifeSpan = 5.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	AActor* ProjectileOwner;

	// Damage dealt by the projectile
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	float Damage = 10.0f;

	UFUNCTION()
	void DestroyProjectile();
};