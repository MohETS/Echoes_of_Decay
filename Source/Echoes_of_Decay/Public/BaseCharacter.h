#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "InventoryComponent.h"
#include "BaseCharacter.generated.h"

class UInventoryWidget;
class UInputMappingContext;
class UInputAction;

UCLASS()
class ECHOES_OF_DECAY_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	UFUNCTION()
	void ToggleInventory();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ToggleInventoryAction;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	UInventoryComponent* Inventory;

public:	
	virtual void Tick(float DeltaTime) override;
};