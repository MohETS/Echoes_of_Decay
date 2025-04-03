#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Weapon/WeaponBase.h"
#include "CollectibleItem.h"
#include "CraftingRecipe.generated.h"

USTRUCT(BlueprintType)
struct ECHOES_OF_DECAY_API FCraftingRecipe : public FTableRowBase
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName CraftingName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ACollectibleItem> ItemA;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ACollectibleItem> ItemB;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AWeaponBase> ResultItem;
};