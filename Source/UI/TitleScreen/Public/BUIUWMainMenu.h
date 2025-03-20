// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BUIUserWidget.h"
#include "BUIUWTitleScreenButton.h"
#include "BUIUWMainMenu.generated.h"


class UBUIUWCreditsScreen;
class UBUIUWSettingsScreen;


/**
 * 
 */
UCLASS()
class UI_API UBUIUWMainMenu : public UBUIUserWidget
{
	GENERATED_BODY()
	
public:

protected:

	/* Variables */

	const FName MainLevel = FName("/Game/Levels/BaseEmptyLevel");


	/* UserWidgets functions */

	virtual void NativeConstruct() override;

	virtual void SynchronizeProperties() override;


	/* Functions */

	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void Settings();

	UFUNCTION()
	void Credits();

	UFUNCTION()
	void Quit();


	/* UI Components*/

	//Buttons
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UBUIUWTitleScreenButton> StartGameButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UBUIUWTitleScreenButton> SettingsButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UBUIUWTitleScreenButton> CreditsButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UBUIUWTitleScreenButton> QuitButton;

	//Credit Page
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBUIUWCreditsScreen> CreditScreenClass;

	UPROPERTY()
	TObjectPtr<UBUIUWCreditsScreen> CreditScreen;

	//Settings Page
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBUIUWSettingsScreen> SettingsScreenClass;

	UPROPERTY()
	TObjectPtr<UBUIUWSettingsScreen> SettingsScreen;

};
