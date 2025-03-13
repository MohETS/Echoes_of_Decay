// Fill out your copyright notice in the Description page of Project Settings.


#include "BUIUWCharacterHUD.h"

void UBUIUWCharacterHUD::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponAbilityContainer1->OnPressedDelegate.AddUniqueDynamic(this, &UBUIUWCharacterHUD::WeaponSelected);
	WeaponAbilityContainer2->OnPressedDelegate.AddUniqueDynamic(this, &UBUIUWCharacterHUD::WeaponSelected);
	WeaponAbilityContainer3->OnPressedDelegate.AddUniqueDynamic(this, &UBUIUWCharacterHUD::WeaponSelected);
}


void UBUIUWCharacterHUD::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}


//Called when the players click on one of his WeaponAbility. For now indicates which WeaponAbility has been "selected"
void UBUIUWCharacterHUD::WeaponSelected(UBUIUWWeaponAbilityContainer* button)
{
	if (button) {
		UE_LOG(LogTemp, Display, TEXT("A WeaponAbility has been selected: %s"), *button->GetWeaponAbilityName().ToString());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("The WeaponAbility has not been initialized"));
	}
}
