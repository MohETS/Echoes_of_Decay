// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BUIUserWidget.h"
#include "BUIUWTitleScreenButton.h"
#include "BUIUWTitleScreen.generated.h"

class UBUIUWMainMenu;

/**
 * 
 */
UCLASS()
class UI_API UBUIUWTitleScreen : public UBUIUserWidget
{
	GENERATED_BODY()
	
public:

protected:

	/* Variables */

	const FName MainLevel = FName("/Game/BaseEmptyLevel");


	/* UserWidgets functions */

	virtual void NativeConstruct() override;

	virtual void SynchronizeProperties() override;


	/* Functions */


	/* UI Components*/


	UPROPERTY(EditAnywhere)
	TSubclassOf<UBUIUWMainMenu> MainMenuClass;

	UPROPERTY()
	TObjectPtr<UBUIUWMainMenu> MainMenu;
};
