#include "Inventory/InventoryWidget/InventorySlotWidget.h"
#include "Inventory/InventoryDragDropOperation.h"
#include "Blueprint/DragDropOperation.h"


void UInventorySlotWidget::SetItem(UInventoryItemWidget* NewItem)
{
    if (!SlotContainer) return;

    ClearSlot();

    ItemWidget = NewItem;

    if (!NewItem) return;

    ItemWidget = NewItem;
    ItemWidget->ParentSlot = this;
    SlotContainer->AddChild(ItemWidget);
}

UInventoryItem* UInventorySlotWidget::GetItem() const
{
    return ItemWidget->ItemData;
}

void UInventorySlotWidget::ClearSlot()
{
    if (SlotContainer && ItemWidget)
    {
        SlotContainer->RemoveChild(ItemWidget);
        ItemWidget = nullptr;
    }
}

bool UInventorySlotWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    UInventoryDragDropOperation* DragOp = Cast<UInventoryDragDropOperation>(InOperation);
    if (DragOp)
    {
        DragOp->TargetWidget = this;
    }

    return true;
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if (!InOperation || !InOperation->Payload) return false;

    UInventoryItemWidget* DraggedItem = Cast<UInventoryItemWidget>(InOperation->Payload);
    if (!DraggedItem || DraggedItem->ParentSlot == this) return false;
    
	// Check if the slot only accepts weapons
    if (bIsWeaponSlotOnly && DraggedItem->ItemData->ItemType == EItemType::Object) return false;

    UInventorySlotWidget* SourceSlot = DraggedItem->ParentSlot;
    if (!SourceSlot) return false;

	// Swap items
    if (ItemWidget)
    {
        SourceSlot->SetItem(ItemWidget);
    }
    else
    {
        SourceSlot->ClearSlot();
    }

    SetItem(DraggedItem);

    return true;
}