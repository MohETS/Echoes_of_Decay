#include "MyCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/ArrowComponent.h"
#include "EnemyBase.h"
#include "Engine/OverlapResult.h"  

AMyCharacter::AMyCharacter()
{
    MyArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("TIRE"));
    MyArrowComponent->SetupAttachment(GetMesh());
    MyArrowComponent->SetHiddenInGame(false);
    MyArrowComponent->SetVisibility(true, true);
   // MyArrowComponent->SetAbsolute(false, true, false);

    Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

    EquippedWeapons.Init(nullptr, 3);

    // Assigner les classes d'arme


    CurrentWeapon = nullptr;

    Health = 100.0f;

    if (GetMesh())
    {
        // Cr�e le ChildActorComponent pour l'arme
        WeaponChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponChildActor"));

        // Attache l'arme au Skeletal Mesh du personnage
        WeaponChildActor->SetupAttachment(GetMesh(), TEXT("items"));
    }

}

void AMyCharacter::BeginPlay()
{
    Super::BeginPlay();

    Health = MaxHealth;
    FRotator CurrentRotation = MyArrowComponent->GetComponentRotation();
    // CurrentRotation.Yaw = 0.0f; // Bloque la rotation autour de l'axe Yaw (Z)
    // MyArrowComponent->SetWorldRotation(CurrentRotation); // Applique cette rotation bloqu�e


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

    if (HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UBUIUWCharacterHUD>(GetWorld(), HUDWidgetClass);
        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();

            HUDWidgetInstance->BindWeaponToHUD(this);
            HUDWidgetInstance->BindHpToHUD(this);
        }
    }

    if (EquippedWeapons[0])
    {
        SwitchToWeapon1();
        UInventoryItem* NewItem = NewObject<UInventoryItem>(Inventory->InventoryWidget);
        NewItem->SetWeaponClass(EquippedWeapons[0]);
        NewItem->ItemWidget = CreateWidget<UInventoryItemWidget>(Inventory->InventoryWidget, Inventory->InventoryItemWidgetClass);
        NewItem->ItemWidget->SetItemData(NewItem, Inventory->InventoryWidget);
        Inventory->InventoryWidget->Weapon1->SetItem(NewItem->ItemWidget);
    }

    RefreshEquippedWeapons();
}

void AMyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    CheckForNearbyEnemies();
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

    GetWorldTimerManager().ClearTimer(RegenTickTimer);
    GetWorldTimerManager().ClearTimer(RegenStartTimer);

    GetWorldTimerManager().SetTimer(RegenStartTimer, this, &AMyCharacter::StartHealthRegen, TimeBeforeRegenStarts, false);

    // V�rification si la sant� est � z�ro
    if (Health <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player Died!"));
        // Ici, tu peux d�clencher une animation de mort, un respawn, etc.
        Destroy(); // Supprime le personnage de la sc�ne
    }

    HUDWidgetInstance->BindHpToHUD(this);

    return DamageAmount;
}

void AMyCharacter::ToggleInventory()
{
    Inventory->ToggleInventory();
}

void AMyCharacter::SwitchWeapon(int32 SlotIndex)
{
    // V�rifie si l'index est valide et si la classe d'arme existe
    if (EquippedWeapons.IsValidIndex(SlotIndex) && EquippedWeapons[SlotIndex])
    {
        if (WeaponChildActor)
        {
            // Change directement la classe du ChildActor
            WeaponChildActor->SetChildActorClass(EquippedWeapons[SlotIndex]);

            // Cr�e une nouvelle instance du ChildActor pour appliquer la nouvelle classe d'arme
            WeaponChildActor->CreateChildActor();

            UE_LOG(LogTemp, Warning, TEXT("Weapon class changed to: %s"), *EquippedWeapons[SlotIndex]->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid weapon slot or class!"));
    }
}


void AMyCharacter::SwitchToWeapon1()
{
    SwitchWeapon(0);
    UE_LOG(LogTemp, Warning, TEXT("Devendra oh yeah"));
    
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
    HUDWidgetInstance->BindWeaponToHUD(this);
}

void AMyCharacter::GainWeaponXP(int32 Xp)
{
    if (CurrentWeapon)
    {
        CurrentWeapon->GainXP(Xp);
    }
}

void AMyCharacter::UseWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Attack();
	}
}

void AMyCharacter::StartHealthRegen()
{
    if (Health >= MaxHealth || bIsEnemyNearby) return;

    GetWorldTimerManager().SetTimer(RegenTickTimer, this, &AMyCharacter::RegenHealth, RegenInterval, true);
}

void AMyCharacter::RegenHealth()
{
    Health = FMath::Clamp(Health + RegenAmount, 0.0f, MaxHealth);
    HUDWidgetInstance->BindHpToHUD(this);

    if (Health >= MaxHealth)
    {
        GetWorldTimerManager().ClearTimer(RegenTickTimer);
    }
}

void AMyCharacter::CheckForNearbyEnemies()
{
    bIsEnemyNearby = false;

    FVector PlayerLocation = GetActorLocation();
    TArray<FOverlapResult> Overlaps;

    FCollisionShape Sphere = FCollisionShape::MakeSphere(EnemyProximityRadius);

    bool bHit = GetWorld()->OverlapMultiByObjectType(
        Overlaps,
        PlayerLocation,
        FQuat::Identity,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
        Sphere
    );

    if (bHit)
    {
        for (auto& Result : Overlaps)
        {
            AActor* OtherActor = Result.GetActor();
            if (OtherActor && OtherActor != this && OtherActor->IsA<AEnemyBase>())
            {
                bIsEnemyNearby = true;
                GetWorldTimerManager().ClearTimer(RegenTickTimer);
                GetWorldTimerManager().ClearTimer(RegenStartTimer);
                GetWorldTimerManager().SetTimer(RegenStartTimer, this, &AMyCharacter::StartHealthRegen, TimeBeforeRegenStarts, false);
                break;
            }
        }
    }
}