#pragma once

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Object UMETA(DisplayName = "Object"),
    Weapon UMETA(DisplayName = "Weapon")
};

UENUM(BlueprintType)
enum class EInventorySlotType : uint8
{
    Default,
    Crafting,
    Result,
    Weapon
};