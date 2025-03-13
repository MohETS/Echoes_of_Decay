#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyBase.generated.h"

UCLASS()
class ECHOES_OF_DECAY_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyBase();
	void Die(AActor* Killer);

protected:
	// Variables li�es � la d�tection de l'IA
	UPROPERTY(EditAnywhere, Category = "AI")
	float SightRadius = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 150.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolRadius = 500.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MoveSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolMaxDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	FVector PatrolCenter;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

	// Variables pour la sant� et les d�g�ts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health = 2.0f;  // Sant� de l'ennemi

	// Timer pour la patrouille et l'attaque
	FTimerHandle PatrolTimer;
	FTimerHandle AttackTimerHandle;

	// Joueur
	APawn* PlayerPawn;

	// Fonction Unreal Engine
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Fonction de perception
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// Fonction de comportement
	void StartAttack();
	void StopAttack();
	void Patrol();
	void ChasePlayer();
	virtual void AttackPlayer();

	// Fonction de r�ception des d�g�ts
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;
};
