#include "BUIUWCharacterHUD.h"
#include "Echoes_of_Decay/Public/MyCharacter.h"


void UBUIUWCharacterHUD::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponAbilityContainer1->OnPressedDelegate.AddUniqueDynamic(this, &UBUIUWCharacterHUD::WeaponSelected);
	WeaponAbilityContainer2->OnPressedDelegate.AddUniqueDynamic(this, &UBUIUWCharacterHUD::WeaponSelected);
	WeaponAbilityContainer3->OnPressedDelegate.AddUniqueDynamic(this, &UBUIUWCharacterHUD::WeaponSelected);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Error getting player controler"));
	}
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

void UBUIUWCharacterHUD::BindWeaponToHUD(AMyCharacter* Player)
{
	WeaponAbilityContainer1->SetUIComponent(Player->EquippedWeapons[0]);
	WeaponAbilityContainer2->SetUIComponent(Player->EquippedWeapons[1]);
	WeaponAbilityContainer3->SetUIComponent(Player->EquippedWeapons[2]);
}

void UBUIUWCharacterHUD::BindHpToHUD(AMyCharacter* Player)
{
	PlayerHealth->SetPercent(Player->Health / Player->MaxHealth);
}