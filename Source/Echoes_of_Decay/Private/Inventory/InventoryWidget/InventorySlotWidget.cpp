#include "Inventory/InventoryWidget/InventorySlotWidget.h"
#include "Inventory/InventoryDragDropOperation.h"
#include "Blueprint/DragDropOperation.h"
#include "Inventory/InventoryWidget/InventoryItemWidget.h"
#include "Inventory/InventoryWidget/InventoryWidget.h"

void UInventorySlotWidget::SetItem(UInventoryItemWidget* NewItem)
{
    if (!SlotContainer) return;

    ClearSlot();

    ItemWidget = NewItem;

    if (!NewItem) return;

    ItemWidget = NewItem;
    ItemWidget->ParentSlot = this;
	OwningInventoryWidget = ItemWidget->OwningInventoryWidget;

    SlotContainer->AddChild(ItemWidget);

    NotifyItemChanged();
}

UInventoryItem* UInventorySlotWidget::GetItem() const
{
    return ItemWidget ? ItemWidget->ItemData : nullptr;
}

void UInventorySlotWidget::ClearSlot()
{
    if (SlotContainer && ItemWidget)
    {
        ItemWidget = nullptr;
        NotifyItemChanged();
    }
}

void UInventorySlotWidget::ClearSlotAfterCraft()
{
	if (SlotContainer && ItemWidget)
	{
		ItemWidget->RemoveFromParent();
		ItemWidget = nullptr;
		NotifyItemChanged();
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

	if ((bIsWeaponSlotOnly && DraggedItem->GetItemType() == EItemType::Object) 
        || (!bIsWeaponSlotOnly && DraggedItem->GetItemType() == EItemType::Weapon)
        || (SlotType == EInventorySlotType::Result && DraggedItem->GetItemType() == EItemType::Weapon))
        return false;

    UInventorySlotWidget* SourceSlot = DraggedItem->ParentSlot;
    if (!SourceSlot || !DraggedItem->ItemData) return false;

    if (bIsWeaponSlotOnly && SourceSlot->SlotType == EInventorySlotType::Result && OwningInventoryWidget)
    {
		UE_LOG(LogTemp, Warning, TEXT("Crafting !"));
        OwningInventoryWidget->ClearCraftingSlotsAfterCraft();
    }

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