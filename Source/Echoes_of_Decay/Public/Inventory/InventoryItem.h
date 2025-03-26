#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItem.generated.h"

class UInventoryItemWidget;

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Object UMETA(DisplayName = "Object"),
    Weapon UMETA(DisplayName = "Weapon")
};

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

    UPROPERTY(BlueprintReadOnly, Category = "UI")
    UInventoryItemWidget* ItemWidget;

    void SetItemWidget(UInventoryItemWidget* NewWidget);
};