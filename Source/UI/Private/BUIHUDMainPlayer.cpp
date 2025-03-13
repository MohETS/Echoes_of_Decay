// Fill out your copyright notice in the Description page of Project Settings.


#include "BUIHUDMainPlayer.h"

// Sets default values
ABUIHUDMainPlayer::ABUIHUDMainPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Init the Character HUD
	CharacterHUDClass = nullptr;
	CharacterHUD = nullptr;

}


// Called when the game starts or when spawned
void ABUIHUDMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	CharacterHUD = CreateWidget<UBUIUWCharacterHUD>(GetWorld(), CharacterHUDClass);
	if (CharacterHUD) {
		CharacterHUD->AddToViewport();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("The CharacterHUD is not initialized"));
	}

}


void ABUIHUDMainPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{


	if (CharacterHUD) {
		CharacterHUD = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}


// Called every frame
void ABUIHUDMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}