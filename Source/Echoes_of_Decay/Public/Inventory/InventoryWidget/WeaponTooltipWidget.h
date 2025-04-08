#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventoryItem.h"
#include "WeaponTooltipWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class ECHOES_OF_DECAY_API UWeaponTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Tooltip")
    void SetWeaponInfo(UInventoryItem* WeaponData);

    UFUNCTION(BlueprintCallable, Category = "Tooltip")
    void ShowAtMousePosition();

    bool bJustAppeared = false;

protected:
    // UI Elements bound from the Widget Blueprint
    UPROPERTY(meta = (BindWidget))
    UTextBlock* WeaponNameText;

    UPROPERTY(meta = (BindWidget))
    UImage* WeaponIconImage;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* WeaponLevelText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* WeaponTypeText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* WeaponEffectText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* WeaponDamageText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* WeaponBuffText;

    FTimerHandle HoverDelayTimer;
    void DelayedShow();
};