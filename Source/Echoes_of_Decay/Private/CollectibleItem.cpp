#include "CollectibleItem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Inventory/InventoryComponent.h"
#include "AkGameplayStatics.h"
#include "../Plugins/WwiseSoundEngine/ThirdParty/include/AK/SoundEngine/Common/AkSoundEngine.h"

// Sets default values
ACollectibleItem::ACollectibleItem()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a sphere component to handle the collision
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    CollisionComponent->SetSphereRadius(50.f);
    RootComponent = CollisionComponent;

	// Bind the overlap event
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACollectibleItem::OnOverlap);

    ItemPickUpSoundPlayingID = AK_INVALID_PLAYING_ID;
}

// Called when the game starts or when spawned
void ACollectibleItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACollectibleItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACollectibleItem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ACharacter* Player = Cast<ACharacter>(OtherActor))
    {
		// Add the item to the player's inventory
        UInventoryComponent* Inventory = Player->FindComponentByClass<UInventoryComponent>();
        if (Inventory)
        {
            Inventory->AddItem(ItemName, ItemIcon, ItemType);
            //TODO Play the sound when object collected
            if (ItemPickUpSound) {
                FOnAkPostEventCallback nullCallback;
                ItemPickUpSoundPlayingID = UAkGameplayStatics::PostEvent(ItemPickUpSound, nullptr, int32(0), nullCallback, false);
            }
            else {
                UE_LOG(LogTemp, Error, TEXT("Wwise Item_Pickup_Sound Event is invalid"));
            }
            Destroy();
        }
    }
}