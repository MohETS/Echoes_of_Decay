#include "BUIUWWeaponAbilityContainer.h"
#include "Echoes_of_Decay/Public/MyCharacter.h"
#include "Echoes_of_Decay/Public/Weapon/WeaponBase.h"
#include "Kismet/GameplayStatics.h"

void UBUIUWWeaponAbilityContainer::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponAbility->OnPressed.AddUniqueDynamic(this, &UBUIUWWeaponAbilityContainer::OnButtonPressed);
}


void UBUIUWWeaponAbilityContainer::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (WeaponAbilityName) 
	{
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

void UBUIUWWeaponAbilityContainer::SetUIComponent(TSubclassOf<AWeaponBase> WeaponClass)
{
    if (!WeaponClass)
    {
        WeaponAbilityImage->SetBrushFromTexture(nullptr);
        WeaponAbilityName->SetText(FText::FromName("No weapon"));
        return;
    }

    AWeaponBase* DefaultWeapon = WeaponClass->GetDefaultObject<AWeaponBase>();
    if (!DefaultWeapon)
    {
        WeaponAbilityImage->SetBrushFromTexture(nullptr);
        WeaponAbilityName->SetText(FText::FromName("No weapon"));
        return;
    }

    if (WeaponAbilityImage)
    {
        WeaponAbilityImage->SetBrushFromTexture(DefaultWeapon->WeaponIcon);
    }

    if (WeaponAbilityName)
    {
        WeaponAbilityName->SetText(FText::FromName(DefaultWeapon->WeaponName));
    }
}