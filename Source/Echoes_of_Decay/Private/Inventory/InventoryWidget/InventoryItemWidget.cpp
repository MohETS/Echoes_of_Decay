#include "Inventory/InventoryWidget/InventoryItemWidget.h"
#include "Inventory/InventoryWidget/InventorySlotWidget.h"
#include "Inventory/InventoryDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


void UInventoryItemWidget::SetItemData(UInventoryItem* Item)
{
    if (!Item) return;

    ItemData = Item;

    if (ItemIcon)
    {
        ItemIcon->SetBrushFromTexture(Item->ItemIcon);
    }

    if (ItemName)
    {
        ItemName->SetText(FText::FromName(Item->ItemName));
    }

    if (ItemData->ItemType == EItemType::Weapon)
    {
        ItemBorder->SetBrushColor(FLinearColor::Red);
    }
    else if (ItemData->ItemType == EItemType::Object)
    {
        ItemBorder->SetBrushColor(FLinearColor::Blue);
    }
}

void UInventoryItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UInventoryItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
    }
    return FReply::Unhandled();
}

void UInventoryItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    if (!ParentSlot) return;

    UInventoryDragDropOperation* DragOperation = NewObject<UInventoryDragDropOperation>();
    if (!DragOperation) return;

    DragOperation->Payload = this;
    DragOperation->DefaultDragVisual = this;
    DragOperation->Pivot = EDragPivot::CenterCenter;
    DragOperation->TargetWidget = nullptr;

    OutOperation = DragOperation;
}

void UInventoryItemWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if (!ParentSlot) return;

    ParentSlot->SetItem(this);
}

void UInventoryItemWidget::ConstrainDrag(UDragDropOperation* Operation)
{
    if (!Operation || !GetParent()) return;

    FVector2D DragPosition = Operation->DefaultDragVisual->GetCachedGeometry().GetAbsolutePosition();
    FVector2D InventoryBounds = GetParent()->GetCachedGeometry().GetAbsoluteSize();

    if (DragPosition.X < 0 || DragPosition.X > InventoryBounds.X ||
        DragPosition.Y < 0 || DragPosition.Y > InventoryBounds.Y)
    {
        ParentSlot->SetItem(this);
        Operation->DefaultDragVisual = nullptr;
    }
}