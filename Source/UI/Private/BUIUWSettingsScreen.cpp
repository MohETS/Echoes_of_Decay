// Fill out your copyright notice in the Description page of Project Settings.


#include "BUIUWSettingsScreen.h"
#include <UI/TitleScreen/Public/BUIUWMainMenu.h>

void UBUIUWSettingsScreen::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->MainButton->OnPressed.AddUniqueDynamic(this, &UBUIUWSettingsScreen::Back);
}


void UBUIUWSettingsScreen::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}


//Closes the Settings menu and brinds the player back to the Main Menu
void UBUIUWSettingsScreen::Back()
{
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
