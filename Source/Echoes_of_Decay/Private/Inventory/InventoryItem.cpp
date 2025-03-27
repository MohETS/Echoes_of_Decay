#include "Inventory/InventoryItem.h"

void UInventoryItem::SetItemWidget(UInventoryItemWidget* NewWidget)
{
	if (!NewWidget) return;

    ItemWidget = NewWidget;
}

void UInventoryItem::SetWeaponClass(TSubclassOf<AWeaponBase> NewWeaponClass)
{
    if (!NewWeaponClass) return;

    WeaponClass = NewWeaponClass;
	ItemName = WeaponClass.GetDefaultObject()->WeaponName;
	ItemIcon = WeaponClass.GetDefaultObject()->WeaponIcon;
	ItemType = WeaponClass.GetDefaultObject()->ItemType;
}