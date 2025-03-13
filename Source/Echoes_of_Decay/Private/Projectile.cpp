#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyBase.h"
#include "GameFramework/DamageType.h"

// Sets default values
AProjectile::AProjectile()
{
    // Enable ticking for this actor
    PrimaryActorTick.bCanEverTick = true;

    // Create and set up the mesh component for the projectile
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // Create and set up the projectile movement component
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 1500.0f;
    ProjectileMovement->MaxSpeed = 1500.0f;
    ProjectileMovement->bRotationFollowsVelocity = true; // Makes the projectile rotate as it moves
    ProjectileMovement->bShouldBounce = false; // Set to true if you want the projectile to bounce
    MeshComponent->SetNotifyRigidBodyCollision(true);

   
   
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
    Super::BeginPlay();
    // Bind the OnHit function to the OnComponentHit event
    MeshComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
}

// Function that gets called when the projectile hits something
void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == this) return;

    UE_LOG(LogTemp, Warning, TEXT("Projectile hit: %s"), *OtherActor->GetName());

    AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);
    if (Enemy)
    {
        UGameplayStatics::ApplyDamage(OtherActor, 10.0f, GetInstigatorController(), this, UDamageType::StaticClass());
    }

    Destroy(); // Toujours détruire le projectile après une collision
}