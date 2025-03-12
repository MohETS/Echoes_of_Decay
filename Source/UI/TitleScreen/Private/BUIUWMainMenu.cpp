// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleScreen/Public/BUIUWMainMenu.h"
#include "BUIUWCreditsScreen.h"
#include "BUIUWSettingsScreen.h"
#include "Kismet/GameplayStatics.h"

void UBUIUWMainMenu::NativeConstruct()
{
	Super::NativeConstruct();


	StartGameButton->MainButton->OnPressed.AddUniqueDynamic(this, &UBUIUWMainMenu::StartGame);
	SettingsButton->MainButton->OnPressed.AddUniqueDynamic(this, &UBUIUWMainMenu::Settings);
	CreditsButton->MainButton->OnPressed.AddUniqueDynamic(this, &UBUIUWMainMenu::Credits);
	QuitButton->MainButton->OnPressed.AddUniqueDynamic(this, &UBUIUWMainMenu::Quit);


}

void UBUIUWMainMenu::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UBUIUWMainMenu::StartGame()
{
	UE_LOG(LogTemp, Display, TEXT("Starting the game"));
	UGameplayStatics::OpenLevel(GetWorld(), MainLevel, false);
}

void UBUIUWMainMenu::Settings()
{
	UE_LOG(LogTemp, Display, TEXT("Going to settings"));

	if (this->IsInViewport()) {
		this->RemoveFromViewport();
	}


	SettingsScreen = CreateWidget<UBUIUWSettingsScreen>(GetWorld(), SettingsScreenClass);
	if (SettingsScreen) {
		SettingsScreen->AddToViewport();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("The SettingScreen is not initialized"));
	}
}

void UBUIUWMainMenu::Credits()
{
	UE_LOG(LogTemp, Display, TEXT("Going to credits"));

	if (this->IsInViewport()) {
		this->RemoveFromViewport();
	}
	
	
	CreditScreen = CreateWidget<UBUIUWCreditsScreen>(GetWorld(), CreditScreenClass);
	if (CreditScreen) {
		CreditScreen->AddToViewport();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("The CreditScreen is not initialized"));
	}
}

void UBUIUWMainMenu::Quit()
{
	UE_LOG(LogTemp, Display, TEXT("Quitting"));
	UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit, false);
}
