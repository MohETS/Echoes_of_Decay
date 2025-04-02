#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
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
	// Damage the projectile inflicts when it hits another actor
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage = 20.0f;

	// Time the projectile will live before being destroyed
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float LifeTime = 5.0f;

	FTimerHandle LifeTimerHandle;

	AActor* Owner;

	// Called when the projectile collides with something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void DestroyProjectile();
};