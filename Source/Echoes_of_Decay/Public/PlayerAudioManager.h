// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AkAudioEvent.h"
#include "AkGameplayStatics.h"
#include "PlayerAudioManager.generated.h"

UCLASS()
class ECHOES_OF_DECAY_API APlayerAudioManager : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerAudioManager();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
	TObjectPtr<UAkAudioEvent> BackgroundMusic;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
	TObjectPtr<UAkStateValue> CalmState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
	TObjectPtr<UAkStateValue> HurtState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
	TObjectPtr<UAkStateValue> DyingState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void PlayBackgroundMusic();
	int32 BackgroundMusicPlayingID;
};
