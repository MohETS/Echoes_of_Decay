#include "MyCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/ArrowComponent.h"

AMyCharacter::AMyCharacter()
{
    MyArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("TIRE"));
    MyArrowComponent->SetupAttachment(GetMesh());
    MyArrowComponent->SetHiddenInGame(false);
    MyArrowComponent->SetVisibility(true, true);
   // MyArrowComponent->SetAbsolute(false, true, false);

    Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

    EquippedWeapons.Init(nullptr, 3);
    CurrentWeapon = nullptr;

    Health = 100.0f;
}

void AMyCharacter::BeginPlay()
{
    Super::BeginPlay();

    FRotator CurrentRotation = MyArrowComponent->GetComponentRotation();
    // CurrentRotation.Yaw = 0.0f; // Bloque la rotation autour de l'axe Yaw (Z)
    // MyArrowComponent->SetWorldRotation(CurrentRotation); // Applique cette rotation bloquée


    // Add the default mapping context to the player's input subsystem
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

        if (InputSubsystem && DefaultMappingContext)
        {
            InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    if (Inventory && Inventory->InventoryWidget && Inventory->InventoryWidget->Weapon1)
    {
        Inventory->InventoryWidget->Weapon1->OnItemChanged.AddDynamic(this, &AMyCharacter::RefreshEquippedWeapons);
        Inventory->InventoryWidget->Weapon2->OnItemChanged.AddDynamic(this, &AMyCharacter::RefreshEquippedWeapons);
        Inventory->InventoryWidget->Weapon3->OnItemChanged.AddDynamic(this, &AMyCharacter::RefreshEquippedWeapons);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Widget or slots are not ready in BeginPlay"));
    }
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (EnhancedInput)
    {
        EnhancedInput->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AMyCharacter::ToggleInventory);
		EnhancedInput->BindAction(AttackAction, ETriggerEvent::Started, this, &AMyCharacter::UseWeapon);
		EnhancedInput->BindAction(WeaponSlot1Action, ETriggerEvent::Started, this, &AMyCharacter::SwitchToWeapon1);
		EnhancedInput->BindAction(WeaponSlot2Action, ETriggerEvent::Started, this, &AMyCharacter::SwitchToWeapon2);
		EnhancedInput->BindAction(WeaponSlot3Action, ETriggerEvent::Started, this, &AMyCharacter::SwitchToWeapon3);
    }
}

// Implémentation de la réception de dégâts
float AMyCharacter::TakeDamage(
    float DamageAmount,
    struct FDamageEvent const& DamageEvent,
    AController* EventInstigator,
    AActor* DamageCauser)
{
    if (DamageAmount <= 0.0f)
    {
        return 0.0f;
    }

    // Réduction de la santé du joueur
    Health -= DamageAmount;
    UE_LOG(LogTemp, Warning, TEXT("Player took damage! Current Health: %f"), Health);

    // Vérification si la santé est à zéro
    if (Health <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player Died!"));
        // Ici, tu peux déclencher une animation de mort, un respawn, etc.
        Destroy(); // Supprime le personnage de la scène
    }

    return DamageAmount;
}

void AMyCharacter::ToggleInventory()
{
    Inventory->ToggleInventory();
}

void AMyCharacter::SwitchWeapon(int32 SlotIndex)
{
    if (EquippedWeapons.IsValidIndex(SlotIndex) && EquippedWeapons[SlotIndex])
    {
        CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(EquippedWeapons[SlotIndex]);
		CurrentWeapon->Owner = this;
        UE_LOG(LogTemp, Warning, TEXT("Switched to weapon: %s"), *CurrentWeapon->WeaponName.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No weapon in this slot!"));
    }
}

void AMyCharacter::SwitchToWeapon1()
{
    SwitchWeapon(0);
}

void AMyCharacter::SwitchToWeapon2()
{
    SwitchWeapon(1);
}

void AMyCharacter::SwitchToWeapon3()
{
    SwitchWeapon(2);
}

void AMyCharacter::RefreshEquippedWeapons()
{
    if (!Inventory || !Inventory->InventoryWidget) return;

    TArray<UInventorySlotWidget*> WeaponSlots = {
        Inventory->InventoryWidget->Weapon1,
        Inventory->InventoryWidget->Weapon2,
        Inventory->InventoryWidget->Weapon3
    };

    EquippedWeapons.Init(nullptr, 3);

    for (int32 i = 0; i < WeaponSlots.Num(); i++)
    {
        UInventorySlotWidget* Slot = WeaponSlots[i];

        if (Slot && Slot->ItemWidget && Slot->ItemWidget->ItemData && Slot->ItemWidget->ItemData->WeaponClass)
        {
            EquippedWeapons[i] = Slot->ItemWidget->ItemData->WeaponClass;
            UE_LOG(LogTemp, Warning, TEXT("Slot %d -> %s"), i + 1, *EquippedWeapons[i]->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Slot %d is empty"), i + 1);
        }
    }
}

void AMyCharacter::UseWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Attack();
	}
}