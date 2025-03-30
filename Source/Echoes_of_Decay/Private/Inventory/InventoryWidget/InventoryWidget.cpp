#include "Inventory/InventoryWidget/InventoryWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "Inventory/InventoryWidget/InventoryItemWidget.h"
#include "Inventory/InventoryWidget/InventorySlotWidget.h"
#include "Inventory/InventoryDragDropOperation.h"
#include "CraftingRecipe.h"

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

    if (!ResultSlot || !Weapon1 || !Weapon2 || !Weapon3 || !SlotA || !SlotB) return;
	ResultSlot->bIsWeaponSlotOnly = true;
	Weapon1->bIsWeaponSlotOnly = true;
	Weapon2->bIsWeaponSlotOnly = true;
    Weapon3->bIsWeaponSlotOnly = true;

    SlotA->OnItemChanged.AddDynamic(this, &UInventoryWidget::OnCraftingSlotsUpdated);
    SlotB->OnItemChanged.AddDynamic(this, &UInventoryWidget::OnCraftingSlotsUpdated);

	ResultSlot->OnItemChanged.AddDynamic(this, &UInventoryWidget::ClearCraftingSlotsAfterCraft);
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
        if (Crafted)
        {
            ResultSlot->SetItem(Crafted->ItemWidget);
        }
    }
}

void UInventoryWidget::ClearCraftingSlotsAfterCraft()
{
    if (!SlotA || !SlotB || !ResultSlot) return;

    SlotA->ClearSlotAfterCraft();
    SlotB->ClearSlotAfterCraft();

    UE_LOG(LogTemp, Warning, TEXT("Crafting slots cleared !"));

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
    if (!TargetSlot
        || (TargetSlot->bIsWeaponSlotOnly && DraggedItem->GetItemType() == EItemType::Object) 
        || (!TargetSlot->bIsWeaponSlotOnly && DraggedItem->GetItemType() == EItemType::Weapon)
        || (TargetSlot->SlotType == EInventorySlotType::Result && DraggedItem->GetItemType() == EItemType::Weapon))
    {
        SourceSlot->SetItem(DraggedItem);
        return false;
    }

    SourceSlot->ClearSlot();
    TargetSlot->SetItem(DraggedItem);
    return true;
}

UInventoryItem* UInventoryWidget::TryCraft(UInventoryItem* ItemA, UInventoryItem* ItemB)
{
    if (!ItemA || !ItemB || !CraftingDataTable) return nullptr;

    UE_LOG(LogTemp, Warning, TEXT("TryCraft"));

    FName NameA = ItemA->ItemName;
    FName NameB = ItemB->ItemName;

    TArray<FCraftingRecipe*> Recipes;
    CraftingDataTable->GetAllRows<FCraftingRecipe>(TEXT("TryCraft"), Recipes);

    for (FCraftingRecipe* Recipe : Recipes)
    {
        if (!Recipe || !Recipe->ItemA || !Recipe->ItemB) continue;

        FName RecipeNameA = Recipe->ItemA->GetDefaultObject<ACollectibleItem>()->ItemName;
        FName RecipeNameB = Recipe->ItemB->GetDefaultObject<ACollectibleItem>()->ItemName;

        UE_LOG(LogTemp, Warning, TEXT("Checking recipe: %s + %s"), *RecipeNameA.ToString(), *RecipeNameB.ToString());

        const bool bMatch =
            (NameA == RecipeNameA && NameB == RecipeNameB) ||
            (NameA == RecipeNameB && NameB == RecipeNameA);

        if (bMatch && Recipe->ResultItem)
        {
            UInventoryItem* NewItem = NewObject<UInventoryItem>(this);
			NewItem->SetWeaponClass(Recipe->ResultItem);
            NewItem->ItemWidget = CreateWidget<UInventoryItemWidget>(this, InventoryItemWidgetClass);
            NewItem->ItemWidget->SetItemData(NewItem, this);
            return NewItem;
        }
    }

    return nullptr;
}