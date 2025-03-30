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


    // Initialisation de la santé
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

    // Assigner l'action de tir au clic gauche
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::FireProjectile);

    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (EnhancedInput && ToggleInventoryAction)
    {
        EnhancedInput->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AMyCharacter::ToggleInventory);
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

        //TODO Play sound of the projectile
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
        //TODO Play player death sound
        Destroy(); // Supprime le personnage de la scène
    }

    return DamageAmount;
}

void AMyCharacter::ToggleInventory()
{
    Inventory->ToggleInventory();
}