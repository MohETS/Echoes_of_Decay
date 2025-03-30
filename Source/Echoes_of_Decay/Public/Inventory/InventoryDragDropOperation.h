#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryDragDropOperation.generated.h"

UCLASS()
class ECHOES_OF_DECAY_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "DragDrop")
	UWidget* TargetWidget;
};
