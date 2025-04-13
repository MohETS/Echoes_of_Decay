// Fill out your copyright notice in the Description page of Project Settings.


#include <Echoes_of_Decay/TitleScreen/Public/TitleScreenSoundManager.h>
#include "AkGameplayStatics.h"
#include "../Plugins/WwiseSoundEngine/ThirdParty/include/AK/SoundEngine/Common/AkSoundEngine.h"

// Sets default values
ATitleScreenSoundManager::ATitleScreenSoundManager()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BackgroundMusicPlayingID = AK_INVALID_PLAYING_ID;
}

// Called when the game starts or when spawned
void ATitleScreenSoundManager::BeginPlay()
{
	Super::BeginPlay();

	PlayBackgroundMusic();
}

void ATitleScreenSoundManager::BeginDestroy()
{
	//Stop playing the background music
	AK::SoundEngine::StopPlayingID(BackgroundMusicPlayingID, 1000, AkCurveInterpolation_Linear);
	Super::BeginDestroy();
}

// Called every frame
void ATitleScreenSoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATitleScreenSoundManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATitleScreenSoundManager::PlayBackgroundMusic() {
	if (BackgroundMusic) {
		FOnAkPostEventCallback nullCallback;
		BackgroundMusicPlayingID = UAkGameplayStatics::PostEvent(BackgroundMusic, this, int32(0), nullCallback, true);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Wwise UI_Background_Music Event is invalid"));
	}
}