// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AkAudioEvent.h"
#include "TitleScreenSoundManager.generated.h"

UCLASS()
class ECHOES_OF_DECAY_API ATitleScreenSoundManager : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATitleScreenSoundManager();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
	TObjectPtr<UAkAudioEvent> BackgroundMusic;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void PlayBackgroundMusic();
	int32 BackgroundMusicPlayingID;

};
