#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventoryItem.h"
#include "InventorySlotWidget.h"
#include "InventoryWidget.generated.h"

class UUniformGridPanel;
class UButton;
class UTextBlock;

UCLASS(Blueprintable)
class ECHOES_OF_DECAY_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 NumSlots = 10;

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    TArray<UInventorySlotWidget*> Slots;

    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* SlotGrid;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* CloseButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TSubclassOf<UInventorySlotWidget> InventorySlotWidgetClass;

    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItemToSlot(UInventoryItemWidget* ItemWidget);

    UFUNCTION()
    void CloseInventory();

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void UpdateInventoryUI();

    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};