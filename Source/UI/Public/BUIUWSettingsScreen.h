// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BUIUserWidget.h"
#include "UI/TitleScreen/Public/BUIUWTitleScreenButton.h"
#include "BUIUWSettingsScreen.generated.h"

class UBUIUWMainMenu;

/**
 * 
 */
UCLASS()
class UI_API UBUIUWSettingsScreen : public UBUIUserWidget
{
	GENERATED_BODY()
	
public:

protected:

	/* UserWidgets functions */

	virtual void NativeConstruct() override;

	virtual void SynchronizeProperties() override;


	/* Functions */

	UFUNCTION()
	void Back();


	/* UI Components*/

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UBUIUWTitleScreenButton> BackButton;


	//Main Menu
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBUIUWMainMenu> MainMenuClass;

	UPROPERTY()
	TObjectPtr<UBUIUWMainMenu> MainMenu;
};
