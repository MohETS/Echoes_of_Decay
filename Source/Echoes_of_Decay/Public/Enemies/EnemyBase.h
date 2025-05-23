#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyBase.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Patrolling,
	Chasing,
	Attacking,
	Dying,
	Idle
};

UCLASS()
class ECHOES_OF_DECAY_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyBase();
	void Die(AActor* Killer);

	float GetDamage() const { return Damage; }

protected:
	// Variables li�es � la d�tection de l'IA
	UPROPERTY(EditAnywhere, Category = "AI")
	float SightRadius = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 150.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolRadius = 500.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float ChaseSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolMaxDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolingTime = 10.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	FVector PatrolCenter;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float EnemyProximityRadius = 1000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	bool bIsPlayerNearby = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Damage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackCooldown = 3.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bCanAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	EEnemyState EnemyState = EEnemyState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int32 XpAtDeath = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UWidgetComponent* HealthBarWidget;

	// Timer pour la patrouille et l'attaque
	FTimerHandle PatrolTimer;
	FTimerHandle AttackTimerHandle;

	AAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regen")
	float TimeBeforeRegenStarts = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regen")
	float RegenAmount = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regen")
	float RegenInterval = 0.5f;

	FTimerHandle RegenStartTimer;
	FTimerHandle RegenTickTimer;

	// Joueur
	APawn* PlayerPawn;

	// Fonction Unreal Engine
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Fonction de perception
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// Fonction de comportement
	void Patrol();
	void ChasePlayer();
	virtual void AttackPlayer();

	void ResetAttackCooldown();

	// Fonction de r�ception des d�g�ts
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;

	UFUNCTION()
	void StartHealthRegen();

	UFUNCTION()
	void RegenHealth();
	
	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar();

	UFUNCTION()
	void CheckForNearbyPlayer();
};
