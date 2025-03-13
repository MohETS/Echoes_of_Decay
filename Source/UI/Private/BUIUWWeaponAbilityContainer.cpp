// Fill out your copyright notice in the Description page of Project Settings.


#include "BUIUWWeaponAbilityContainer.h"

void UBUIUWWeaponAbilityContainer::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponAbility->OnPressed.AddUniqueDynamic(this, &UBUIUWWeaponAbilityContainer::OnButtonPressed);
}


void UBUIUWWeaponAbilityContainer::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (WeaponAbilityName) {
		WeaponAbilityName->SetText(WeaponAbilityNameLabelText);
	}

}


void UBUIUWWeaponAbilityContainer::OnButtonPressed()
{
	OnPressedDelegate.Broadcast(this);
}


FText UBUIUWWeaponAbilityContainer::GetWeaponAbilityName()
{
	return WeaponAbilityNameLabelText;
}