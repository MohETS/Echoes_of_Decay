#include "InventoryComponent.h"
#include "InventoryItem.h"
#include "InventoryItemWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

    if (InventoryWidgetClass)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            InventoryWidget = CreateWidget<UInventoryWidget>(PlayerController, InventoryWidgetClass);

            if (InventoryWidget)
            {
                InventoryWidget->AddToViewport();
                InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
            }
        }
    }
}

UInventoryItem* UInventoryComponent::AddItem(FName ItemName, UTexture2D* ItemIcon, EItemType ItemType)
{
    if (!InventoryWidget || !InventoryItemWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("InventoryWidget or InventoryItemWidgetClass is NULL!"));
        return nullptr;
    }

    UInventoryItem* NewItem = NewObject<UInventoryItem>(this);
    if (!NewItem)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create InventoryItem!"));
        return nullptr;
    }

    NewItem->ItemName = ItemName;
    NewItem->ItemIcon = ItemIcon;
    NewItem->ItemType = ItemType;
    Items.Add(NewItem);

    UE_LOG(LogTemp, Warning, TEXT("Item Created: %s"), *ItemName.ToString());

    UInventoryItemWidget* ItemWidget = CreateWidget<UInventoryItemWidget>(InventoryWidget, InventoryItemWidgetClass);
    if (!ItemWidget)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create InventoryItemWidget!"));
        return nullptr;
    }
    
    if (ItemWidget)
    {
        ItemWidget->SetItemData(NewItem);
        NewItem->SetItemWidget(ItemWidget);

        if (!InventoryWidget->AddItemToSlot(ItemWidget))
        {
            UE_LOG(LogTemp, Warning, TEXT("No available slots in inventory!"));
        }
    }

    return NewItem;
}

void UInventoryComponent::ToggleInventory()
{
    if (!InventoryWidget) return;

    InventoryWidget->SetVisibility(ESlateVisibility::Visible);
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = true;
    }
}