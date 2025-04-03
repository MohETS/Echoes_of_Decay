#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapon/WeaponBase.h"
#include "Inventory/ItemType.h"
#include "InventoryItem.generated.h"

class UInventoryItemWidget;

UCLASS(Blueprintable)
class ECHOES_OF_DECAY_API UInventoryItem : public UObject
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* ItemIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeaponBase> WeaponClass;

    UPROPERTY(BlueprintReadOnly, Category = "UI")
    UInventoryItemWidget* ItemWidget;

    void SetItemWidget(UInventoryItemWidget* NewWidget);
	void SetWeaponClass(TSubclassOf<AWeaponBase> NewWeaponClass);
};