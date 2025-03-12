// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleScreenCameraPawn.h"
#include <UI/TitleScreen/Public/BUIUWTitleScreen.h>
#include "BUIUserWidget.h"

// Sets default values
ATitleScreenCameraPawn::ATitleScreenCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TitleScreenClass = nullptr;
	TitleScreen = nullptr;

}

// Called when the game starts or when spawned
void ATitleScreenCameraPawn::BeginPlay()
{
	Super::BeginPlay();

	if (TitleScreenClass && GetWorld()) {
		TitleScreen = CreateWidget<UBUIUWTitleScreen>(GetWorld(), TitleScreenClass);
		check(TitleScreen);
		TitleScreen->AddToViewport();
	}
	
}

void ATitleScreenCameraPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	

	if (TitleScreen) {
		TitleScreen->RemoveFromViewport();
		TitleScreen = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ATitleScreenCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATitleScreenCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

