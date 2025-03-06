#include "BaseCharacter.h"
#include "InventoryWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
    Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add the default mapping context to the player's input subsystem
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

        if (InputSubsystem && DefaultMappingContext)
        {
            InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (EnhancedInput && ToggleInventoryAction)
    {
        EnhancedInput->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &ABaseCharacter::ToggleInventory);
    }
}

void ABaseCharacter::ToggleInventory()
{
    Inventory->ToggleInventory();
}