// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BUIUserWidget.h"
#include "BUIUWWeaponAbilityContainer.h"
#include <Components/ProgressBar.h>
#include "BUIUWCharacterHUD.generated.h"

/**
 * 
 */
UCLASS()
class UI_API UBUIUWCharacterHUD : public UBUIUserWidget
{
	GENERATED_BODY()
	
public:

protected:

	/* Variables */


	/* UserWidgets functions */

	virtual void NativeConstruct() override;

	virtual void SynchronizeProperties() override;


	/* Functions */
	UFUNCTION()
	void WeaponSelected(UBUIUWWeaponAbilityContainer* button);

	/* UI Components*/

	//Player WeaponsAbility
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UBUIUWWeaponAbilityContainer> WeaponAbilityContainer1;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UBUIUWWeaponAbilityContainer> WeaponAbilityContainer2;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UBUIUWWeaponAbilityContainer> WeaponAbilityContainer3;

	//Player Health bar
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> PlayerHealth;

	/*UPROPERTY(EditAnywhere)
	TSubclassOf<UBUIUWMainMenu> MainMenuClass;

	UPROPERTY()
	TObjectPtr<UBUIUWMainMenu> MainMenu;*/
};
