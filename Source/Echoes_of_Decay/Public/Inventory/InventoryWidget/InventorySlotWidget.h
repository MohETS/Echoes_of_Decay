#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.h"
#include "Components/Overlay.h"
#include "InventorySlotWidget.generated.h"

UCLASS()
class ECHOES_OF_DECAY_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    UOverlay* SlotContainer;

    UPROPERTY(BlueprintReadOnly, Category = "Slot")
    UInventoryItemWidget* ItemWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bIsWeaponSlotOnly = false;

    UFUNCTION(BlueprintCallable, Category = "Slot")
    bool IsEmpty() const { return ItemWidget == nullptr; }

    UFUNCTION(BlueprintCallable, Category = "Slot")
    void SetItem(UInventoryItemWidget* NewItem);

    UFUNCTION(BlueprintCallable, Category = "Slot")
    UInventoryItem* GetItem() const;

    UFUNCTION(BlueprintCallable, Category = "Slot")
    void ClearSlot();

    virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};