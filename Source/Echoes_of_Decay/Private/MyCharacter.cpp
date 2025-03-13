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

    // Initialisation de la sant�
    Health = 100.0f;
}

void AMyCharacter::BeginPlay()
{
    Super::BeginPlay();

    FRotator CurrentRotation = MyArrowComponent->GetComponentRotation();
  //  CurrentRotation.Yaw = 0.0f; // Bloque la rotation autour de l'axe Yaw (Z)
  //  MyArrowComponent->SetWorldRotation(CurrentRotation); // Applique cette rotation bloqu�e
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Assigner l'action de tir au clic gauche
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::FireProjectile);
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

// Impl�mentation de la r�ception de d�g�ts
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

    // R�duction de la sant� du joueur
    Health -= DamageAmount;
    UE_LOG(LogTemp, Warning, TEXT("Player took damage! Current Health: %f"), Health);

    // V�rification si la sant� est � z�ro
    if (Health <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player Died!"));
        // Ici, tu peux d�clencher une animation de mort, un respawn, etc.
        Destroy(); // Supprime le personnage de la sc�ne
    }

    return DamageAmount;
}
