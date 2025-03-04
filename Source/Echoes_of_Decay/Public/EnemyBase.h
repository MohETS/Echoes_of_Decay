// Fill out your copyright notice in the Description page of Project Settings.

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

protected:
    // Behaviour and detection variable
    UPROPERTY(EditAnywhere, Category = "AI")
    float SightRadius = 1000.0f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackRange = 150.0f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float PatrolRadius = 500.0f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float MoveSpeed = 300.0f;

    UPROPERTY(VisibleAnywhere, Category = "AI")
    UAIPerceptionComponent* AIPerceptionComponent;

    UPROPERTY(VisibleAnywhere, Category = "AI")
    UAISenseConfig_Sight* SightConfig;

    // Patrol timer
    FTimerHandle PatrolTimer;

    // Player
    APawn* PlayerPawn;

	// Attack timer
    FTimerHandle AttackTimerHandle;

	// Unreal Engine functions
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
	// Perception function
    UFUNCTION()
    void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// Behaviour functions
    void StartAttack();
    void StopAttack();
    void Patrol();
    void ChasePlayer();
    virtual void AttackPlayer();
};