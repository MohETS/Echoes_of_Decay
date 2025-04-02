#include "InventoryWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "InventoryItemWidget.h"
#include "InventorySlotWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "InventoryDragDropOperation.h"

void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CloseButton)
    {
        CloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::CloseInventory);
    }

    if (!SlotGrid) return;
    if (!InventorySlotWidgetClass) return;

    for (int32 i = 0; i < NumSlots; i++)
    {
        UInventorySlotWidget* NewSlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
        if (!NewSlot) continue;
        Slots.Add(NewSlot);
        SlotGrid->AddChildToUniformGrid(NewSlot, i / 5, i % 5);
    }
}

void UInventoryWidget::CloseInventory()
{
    SetVisibility(ESlateVisibility::Hidden);

    APlayerController* PlayerController = GetOwningPlayer();
    if (PlayerController)
    {
        PlayerController->SetInputMode(FInputModeGameOnly());
        PlayerController->bShowMouseCursor = false;
    }
}

bool UInventoryWidget::AddItemToSlot(UInventoryItemWidget* ItemWidget)
{
    for (UInventorySlotWidget* S : Slots)
    {
        if (S && S->IsEmpty())
        {
            ItemWidget->ParentSlot = S;
            S->SetItem(ItemWidget);
            return true;
        }
    }
    return false;
}

void UInventoryWidget::UpdateInventoryUI()
{
    for (int32 i = 0; i < Slots.Num(); i++)
    {
        if (Slots[i] && Slots[i]->ItemWidget)
        {
            SlotGrid->AddChildToUniformGrid(Slots[i]->ItemWidget, i / 5, i % 5);
        }
    }
}

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if (!InOperation || !InOperation->Payload) return false;

    UInventoryDragDropOperation* DragOp = Cast<UInventoryDragDropOperation>(InOperation);
    if (!DragOp) return false;

    UInventoryItemWidget* DraggedItem = Cast<UInventoryItemWidget>(DragOp->Payload);
    if (!DraggedItem || !DraggedItem->ParentSlot) return false;

    UInventorySlotWidget* SourceSlot = DraggedItem->ParentSlot;
    UInventorySlotWidget* TargetSlot = Cast<UInventorySlotWidget>(DragOp->TargetWidget);
    if (!TargetSlot)
    {
        SourceSlot->SetItem(DraggedItem);
        return false;
    }

    SourceSlot->ClearSlot();
    TargetSlot->SetItem(DraggedItem);

    return true;
}
