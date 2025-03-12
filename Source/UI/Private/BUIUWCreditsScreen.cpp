// Fill out your copyright notice in the Description page of Project Settings.


#include "BUIUWCreditsScreen.h"
#include <UI/TitleScreen/Public/BUIUWMainMenu.h>

void UBUIUWCreditsScreen::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->MainButton->OnPressed.AddUniqueDynamic(this, &UBUIUWCreditsScreen::Back);
}

void UBUIUWCreditsScreen::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UBUIUWCreditsScreen::Back()
{
	UE_LOG(LogTemp, Display, TEXT("Backing from credits page"));

	if (this->IsInViewport()) {
		this->RemoveFromViewport();
	}


	MainMenu = CreateWidget<UBUIUWMainMenu>(GetWorld(), MainMenuClass);
	if (MainMenu) {
		MainMenu->AddToViewport();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("The MainMenu is not initialized"));
	}
}
