#include "Inventory/InventoryWidget/InventoryWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "Inventory/InventoryWidget/InventoryItemWidget.h"
#include "Inventory/InventoryWidget/InventorySlotWidget.h"
#include "Inventory/InventoryDragDropOperation.h"

void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!CloseButton) return;
    CloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::CloseInventory);

    if (!InventorySlotWidgetClass || !SlotGrid) return;

    for (int32 i = 0; i < NumSlots; i++)
    {
        UInventorySlotWidget* NewSlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
        if (!NewSlot) continue;
        Slots.Add(NewSlot);
        SlotGrid->AddChildToUniformGrid(NewSlot, i / 5, i % 5);
    }

    if (!ResultSlot || !Weapon1 || !Weapon2 || !Weapon3) return;
	ResultSlot->bIsWeaponSlotOnly = true;
	Weapon1->bIsWeaponSlotOnly = true;
	Weapon2->bIsWeaponSlotOnly = true;
    Weapon3->bIsWeaponSlotOnly = true;
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

void UInventoryWidget::OnCraftingSlotsUpdated()
{
    if (!SlotA || !SlotB || !ResultSlot) return;

    UInventoryItem* ItemA = SlotA->GetItem();
    UInventoryItem* ItemB = SlotB->GetItem();

    if (ItemA && ItemB)
    {
        UInventoryItem* Crafted = TryCraft(ItemA, ItemB);
        ResultSlot->SetItem(Crafted->ItemWidget);
    }
    else
    {
        ResultSlot->ClearSlot();
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
    if (!TargetSlot || !TargetSlot->bIsWeaponSlotOnly)
    {
        SourceSlot->SetItem(DraggedItem);
        return false;
    }

    SourceSlot->ClearSlot();
    TargetSlot->SetItem(DraggedItem);
	OnCraftingSlotsUpdated();

    return true;
}

UInventoryItem* UInventoryWidget::TryCraft(UInventoryItem* ItemA, UInventoryItem* ItemB)
{
    if (!ItemA || !ItemB) return nullptr;

	// Crafting logic here

    return nullptr;
}
