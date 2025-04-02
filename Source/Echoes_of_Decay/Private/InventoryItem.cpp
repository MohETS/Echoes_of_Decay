#include "InventoryItem.h"

void UInventoryItem::SetItemWidget(UInventoryItemWidget* NewWidget)
{
    if (NewWidget)
    {
        ItemWidget = NewWidget;
    }
}