// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleScreen/Public/BUIUWTitleScreen.h"
#include "UI/TitleScreen/Public/BUIUWMainMenu.h"

void UBUIUWTitleScreen::NativeConstruct()
{
	Super::NativeConstruct();

	MainMenu = CreateWidget<UBUIUWMainMenu>(GetWorld(), MainMenuClass);
	if (MainMenu) {
		MainMenu->AddToViewport();

        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            PlayerController->bShowMouseCursor = true;
            FInputModeGameAndUI InputMode;
            InputMode.SetWidgetToFocus(MainMenu->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PlayerController->SetInputMode(InputMode);
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("Error getting player controler"));
        }
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("The MainMenu is not initialized"));
	}
}

void UBUIUWTitleScreen::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}
