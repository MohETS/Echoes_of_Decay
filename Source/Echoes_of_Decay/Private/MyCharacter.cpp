#include "MyCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/ArrowComponent.h"
#include "Enemies/EnemyBase.h"
#include "Animation/AnimMontage.h"
#include "Engine/OverlapResult.h"  

#include "AkGameplayStatics.h"
#include "../Plugins/WwiseSoundEngine/ThirdParty/include/AK/SoundEngine/Common/AkSoundEngine.h"


AMyCharacter::AMyCharacter()
{
	MyArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("TIRE"));
	MyArrowComponent->SetupAttachment(GetMesh());
	MyArrowComponent->SetHiddenInGame(false);
	MyArrowComponent->SetVisibility(false, false);

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	EquippedWeapons.Init(nullptr, 3);

	CurrentWeapon = nullptr;

	Health = 100.0f;

	if (GetMesh())
	{
		WeaponChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponChildActor"));
		WeaponChildActor->SetupAttachment(GetMesh(), TEXT("items"));
	}
	PlayerDeathSoundPlayingID = AK_INVALID_PLAYING_ID;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	FRotator CurrentRotation = MyArrowComponent->GetComponentRotation();

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

	if (StartingWeapon)
	{
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(StartingWeapon);
		if (CurrentWeapon)
		{
			EquippedWeapons[0] = CurrentWeapon;
			CurrentWeapon->Owner = this;
			SwitchToWeapon1();
			UInventoryItem* NewItem = NewObject<UInventoryItem>(Inventory->InventoryWidget);
			NewItem->SetWeaponClass(EquippedWeapons[0]->GetClass());
			NewItem->ItemWidget = CreateWidget<UInventoryItemWidget>(Inventory->InventoryWidget, Inventory->InventoryItemWidgetClass);
			NewItem->ItemWidget->SetItemData(NewItem, Inventory->InventoryWidget);
			Inventory->InventoryWidget->Weapon1->SetItem(NewItem->ItemWidget);
		}
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

float AMyCharacter::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage <= 0.0f)
	{
		return 0.0f;
	}

	Health -= ActualDamage;

	GetWorldTimerManager().ClearTimer(RegenTickTimer);
	GetWorldTimerManager().ClearTimer(RegenStartTimer);

	GetWorldTimerManager().SetTimer(RegenStartTimer, this, &AMyCharacter::StartHealthRegen, TimeBeforeRegenStarts, false);
	
    if (Health <= 0.0f && backgroundMusicPlayerState != DEAD)
    {
		backgroundMusicPlayerState = playerMusicState::DEAD;
        if (PlayerDeathSound) {
            FOnAkPostEventCallback nullCallback;
            PlayerDeathSoundPlayingID = UAkGameplayStatics::PostEvent(PlayerDeathSound, this, int32(0), nullCallback, false);
        }
        else 
		{
            UE_LOG(LogTemp, Error, TEXT("Wwise Player_Background_Music Event is invalid"));
        }

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		DisableInput(PlayerController);
		PlayAnimMontage(DeathMontage);
		if (GameOverScreen) {
			GameOverScreen->AddToViewport();
		}
		PlayerController->bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(GameOverScreen->GetSlateWidgetFromName("Respawn button"));
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
    }

	//When the players loses health the background music changes to the "Hurt" section of the music
	if ((Health < MaxHealth && Health >= 50.0f) && backgroundMusicPlayerState != playerMusicState::HURT) {
		backgroundMusicPlayerState = playerMusicState::HURT;
		UAkGameplayStatics::SetState(HurtState);
	}

	//When the players loses half his health the background music changes to the "Dying" section of the music
	if (Health > 0.0f && Health <= MaxHealth / 2 && backgroundMusicPlayerState != playerMusicState::DYING) {
		backgroundMusicPlayerState = playerMusicState::DYING;
		UAkGameplayStatics::SetState(DyingState);
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
	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon->SetActorEnableCollision(false);
	}

	if (EquippedWeapons.IsValidIndex(SlotIndex) && EquippedWeapons[SlotIndex])
	{
		CurrentWeapon = EquippedWeapons[SlotIndex];
		CurrentWeapon->SetActorHiddenInGame(false);
		CurrentWeapon->SetActorEnableCollision(true);
		if (!WeaponChildActor) return;
		WeaponChildActor->SetChildActorClass(EquippedWeapons[SlotIndex]->GetClass());
		WeaponChildActor->CreateChildActor();
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
		if (!Slot || !Slot->ItemWidget || !Slot->ItemWidget->ItemData || !Slot->ItemWidget->ItemData->WeaponClass) continue;

		AWeaponBase* WeaponInstance = GetWorld()->SpawnActor<AWeaponBase>(Slot->ItemWidget->ItemData->WeaponClass);
		if (!WeaponInstance) continue;

		WeaponInstance->Owner = this;
		EquippedWeapons[i] = WeaponInstance;
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

	//Plays back the calm music when the player health is above a certain threshold
	if (backgroundMusicPlayerState != CALM) {
		backgroundMusicPlayerState = CALM;
		UAkGameplayStatics::SetState(CalmState);
	}

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

	if (!bHit) return;
	for (auto& Result : Overlaps)
	{
		AActor* OtherActor = Result.GetActor();
		if (!OtherActor || OtherActor == this || !OtherActor->IsA<AEnemyBase>()) continue;
		bIsEnemyNearby = true;
		GetWorldTimerManager().ClearTimer(RegenTickTimer);
		GetWorldTimerManager().ClearTimer(RegenStartTimer);
		GetWorldTimerManager().SetTimer(RegenStartTimer, this, &AMyCharacter::StartHealthRegen, TimeBeforeRegenStarts, false);
		break;
	}
}