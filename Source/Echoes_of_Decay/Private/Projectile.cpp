#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyBase.h"
#include "EnemyRanged.h"
#include "Weapon/RangedWeapon.h"
#include "GameFramework/DamageType.h"

AProjectile::AProjectile()
{
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
    ProjectileMovement->UpdatedComponent = CollisionComponent;
    ProjectileMovement->InitialSpeed = 2000.f;
    ProjectileMovement->MaxSpeed = 2000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;

    InitialLifeSpan = LifeSpan;
    SetLifeSpan(LifeSpan);
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

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AProjectile::DestroyProjectile()
{
	Destroy();
}
