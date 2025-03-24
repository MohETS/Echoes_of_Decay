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
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::FireProjectile);

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

void AMyCharacter::FireProjectile()
{
    if (ProjectileClass && MyArrowComponent)
    {
        // Obtenir la transformation de l'ArrowComponent (position et rotation)
        FTransform SpawnTransform = MyArrowComponent->GetComponentTransform();

        // Spawner le projectile
        GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform);
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


void AMyCharacter::UseWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Attack();
	}
}