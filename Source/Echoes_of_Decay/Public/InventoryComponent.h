#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryWidget.h"
#include "InventoryItem.h"
#include "InventoryItemWidget.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECHOES_OF_DECAY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UInventoryItemWidget> InventoryItemWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UInventoryWidget> InventoryWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "UI")
    UInventoryWidget* InventoryWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TArray<UInventoryItem*> Items;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    UInventoryItem* AddItem(FName ItemName, UTexture2D* ItemIcon, EItemType ItemType);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void ToggleInventory();
};