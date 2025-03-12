#include "MyCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/ArrowComponent.h"  

AMyCharacter::AMyCharacter()
{
    MyArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("TIRE"));
    MyArrowComponent->SetupAttachment(GetMesh(), TEXT("LeftHandEmplacement"));
    MyArrowComponent->SetHiddenInGame(false); // S'assurer qu'il est visible en jeu
    MyArrowComponent->SetVisibility(true, true);
    MyArrowComponent->SetAbsolute(false, true, false);
}

void AMyCharacter::BeginPlay()
{
    Super::BeginPlay();
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
