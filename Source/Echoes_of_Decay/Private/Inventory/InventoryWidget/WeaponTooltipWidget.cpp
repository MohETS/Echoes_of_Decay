#include "Inventory/InventoryWidget/WeaponTooltipWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/MeleeWeapon.h"
#include "Weapon/RangedWeapon.h"
#include "Weapon/BuffWeapon.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void UWeaponTooltipWidget::SetWeaponInfo(UInventoryItem* WeaponData)
{
    if (!WeaponData || !WeaponData->WeaponClass) return;

    AWeaponBase* DefaultWeapon = WeaponData->WeaponClass->GetDefaultObject<AWeaponBase>();
    if (!DefaultWeapon) return;

    if (WeaponNameText)
    {
        WeaponNameText->SetText(FText::FromName(DefaultWeapon->WeaponName));
    }

    if (WeaponIconImage)
    {
        WeaponIconImage->SetBrushFromTexture(DefaultWeapon->WeaponIcon);
    }

    if (WeaponLevelText)
    {
        WeaponLevelText->SetText(FText::AsNumber(DefaultWeapon->WeaponLevel));
    }

    if (WeaponTypeText && WeaponDamageText && WeaponBuffText)
    {
        FString WeaponTypeStr;
        FString WeaponDamageStr;
        FString WeaponBuffStr;
        switch (DefaultWeapon->WeaponType)
        {
            case EWeaponType::Melee:
                WeaponTypeStr = TEXT("Melee");
                WeaponDamageStr = FString::FormatAsNumber(Cast<AMeleeWeapon>(DefaultWeapon)->AttackDamage);
                WeaponBuffStr = "No buff";
                break;
            case EWeaponType::Ranged:
                WeaponTypeStr = TEXT("Ranged");
                WeaponDamageStr = FString::FormatAsNumber(Cast<ARangedWeapon>(DefaultWeapon)->ProjectileDamage);
                WeaponBuffStr = "No buff";
                break;
            case EWeaponType::Buff:
                WeaponTypeStr = TEXT("Buff");
                WeaponDamageStr = FString::FormatAsNumber(0);
                WeaponBuffStr = FString::SanitizeFloat(Cast<ABuffWeapon>(DefaultWeapon)->BuffMultiplier);
                break;
            default:
                WeaponTypeStr = TEXT("Unknown Type");
                break;
        }

        WeaponTypeText->SetText(FText::FromString(WeaponTypeStr));
        WeaponDamageText->SetText(FText::FromString(WeaponDamageStr));
        WeaponBuffText->SetText(FText::FromString(WeaponBuffStr));
    }

    if (WeaponEffectText)
    {
        WeaponEffectText->SetText(FText::FromName(DefaultWeapon->WeaponEffect->EffectName));
    }
}

void UWeaponTooltipWidget::ShowAtMousePosition()
{
    GetWorld()->GetTimerManager().SetTimer(HoverDelayTimer, this, &UWeaponTooltipWidget::DelayedShow, 0.2f, false);
}

void UWeaponTooltipWidget::DelayedShow()
{
    FVector2D MousePos;
    UWidgetLayoutLibrary::GetMousePositionScaledByDPI(UGameplayStatics::GetPlayerController(GetWorld(), 0), MousePos.X, MousePos.Y);
    SetPositionInViewport(MousePos, true);
    SetVisibility(ESlateVisibility::Visible);
}