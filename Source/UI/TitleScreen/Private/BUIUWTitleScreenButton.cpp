// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleScreen/Public/BUIUWTitleScreenButton.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkGameplayStatics.h"

void UBUIUWTitleScreenButton::NativeConstruct()
{
	Super::NativeConstruct();
	MainButton->OnHovered.AddUniqueDynamic(this, &UBUIUWTitleScreenButton::PlayHoveredSound);
}

void UBUIUWTitleScreenButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (ButtonLabel) {
		ButtonLabel->SetText(ButtonLabelText);
	}

}

void UBUIUWTitleScreenButton::PlayHoveredSound() {
	int32 PlayingID = AK_INVALID_PLAYING_ID;
	if (ButtonSelectedSound) {
		FOnAkPostEventCallback nullCallback;
		PlayingID = UAkGameplayStatics::PostEvent(ButtonSelectedSound, nullptr, int32(0), nullCallback, true);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Wwise Event is invalid"));
	}
	UE_LOG(LogTemp, Display, TEXT("Is Hovered"));
}