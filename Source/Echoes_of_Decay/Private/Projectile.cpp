#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemies/EnemyBase.h"
#include "Enemies/EnemyRanged.h"
#include "Weapon/RangedWeapon.h"
#include "GameFramework/DamageType.h"

// Sets default values
AProjectile::AProjectile()
{
    // Enable ticking for this actor
    PrimaryActorTick.bCanEverTick = true;

    // Collision Component (Root)
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    CollisionComponent->SetGenerateOverlapEvents(true);

    // Movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 1500.0f;
    ProjectileMovement->MaxSpeed = 1500.0f;
    ProjectileMovement->bRotationFollowsVelocity = true; // Makes the projectile rotate as it moves
    ProjectileMovement->bShouldBounce = false; // Set to true if you want the projectile to bounce
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
    Super::BeginPlay();

    if (GetOwner())
    {
        AEnemyRanged* O = Cast<AEnemyRanged>(GetOwner());
        if (!O) return;
        Damage = O->GetDamage();
    }
} 

void AProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
}

void AProjectile::DestroyProjectile()
{
	Destroy();
}
