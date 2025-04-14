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


//Closes the Credits menu and gets the player back to the Main Menu
void UBUIUWCreditsScreen::Back()
{
	if (this->IsInViewport()) {
		this->RemoveFromParent();
	}


	MainMenu = CreateWidget<UBUIUWMainMenu>(GetWorld(), MainMenuClass);
	if (MainMenu) {
		MainMenu->AddToViewport();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("The MainMenu is not initialized"));
	}
}
