#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "InventoryItem.h"
#include "InventoryItemWidget.generated.h"

class UInventorySlotWidget;

UCLASS()
class ECHOES_OF_DECAY_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "Item")
    UInventoryItem * ItemData;

    UPROPERTY(meta = (BindWidget))
    UImage* ItemIcon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemName;

    UPROPERTY(meta = (BindWidget))
    UBorder* ItemBorder;

    UPROPERTY(BlueprintReadOnly, Category = "Item")
    UInventorySlotWidget* ParentSlot;

    UFUNCTION(BlueprintCallable, Category = "Item")
    void SetItemData(UInventoryItem* Item);

    UFUNCTION()
    void ConstrainDrag(UDragDropOperation* Operation);

protected:
    virtual void NativeConstruct() override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
};
