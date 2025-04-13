// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAudioManager.h"
#include "../Plugins/WwiseSoundEngine/ThirdParty/include/AK/SoundEngine/Common/AkSoundEngine.h"

// Sets default values
APlayerAudioManager::APlayerAudioManager()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BackgroundMusicPlayingID = AK_INVALID_PLAYING_ID;
}

// Called when the game starts or when spawned
void APlayerAudioManager::BeginPlay()
{
	Super::BeginPlay();

	UAkGameplayStatics::SetOcclusionRefreshInterval(0, this);
	PlayBackgroundMusic();
}

// Called every frame
void APlayerAudioManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerAudioManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerAudioManager::PlayBackgroundMusic() {
	if (BackgroundMusic) {
		FOnAkPostEventCallback nullCallback;
		UAkGameplayStatics::SetState(CalmState);
		BackgroundMusicPlayingID = UAkGameplayStatics::PostEvent(BackgroundMusic, this, int32(0), nullCallback, true);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Wwise Player_Background_Music Event is invalid"));
	}
}