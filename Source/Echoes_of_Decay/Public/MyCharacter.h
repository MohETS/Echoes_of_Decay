#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Projectile.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "Inventory/InventoryComponent.h"
#include "Weapon/WeaponBase.h"
#include "UI/Public/BUIUWCharacterHUD.h"
#include "Animation/AnimMontage.h"
#include "Components/ChildActorComponent.h"
#include "AkGameplayStatics.h"
#include "MyCharacter.generated.h"

class UInventoryWidget;
class UInputMappingContext;
class UInputAction;

UCLASS()
class ECHOES_OF_DECAY_API AMyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMyCharacter();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SoundFX")
    TObjectPtr<UAkAudioEvent> PlayerDeathSound;

    int32 PlayerDeathSoundPlayingID;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// --- COMBAT --- //

	// Projectile class to spawn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TSubclassOf<class AProjectile> ProjectileClass;

	// Arrow component to specify the spawn location of the projectile
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    class UArrowComponent* MyArrowComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float EnemyProximityRadius = 1000.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsEnemyNearby = false;

    UFUNCTION()
    void CheckForNearbyEnemies();

	// --- INPUT --- //
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* ToggleInventoryAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* AttackAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* WeaponSlot1Action;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* WeaponSlot2Action;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* WeaponSlot3Action;

	// --- INVENTORY --- //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    UChildActorComponent* WeaponChildActor;

    UPROPERTY(VisibleAnywhere, Category = "Inventory")
    UInventoryComponent* Inventory;

    UFUNCTION()
    void ToggleInventory();

	// --- WEAPON --- //

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TArray<AWeaponBase*> EquippedWeapons;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AWeaponBase> StartingWeapon;

    UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category = "Weapon")
    AWeaponBase* CurrentWeapon;

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SwitchWeapon(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void UseWeapon();

    UFUNCTION()
    void SwitchToWeapon1();

    UFUNCTION()
    void SwitchToWeapon2();

    UFUNCTION()
    void SwitchToWeapon3();

    UFUNCTION()
    void RefreshEquippedWeapons();

    UFUNCTION()
    void GainWeaponXP(int32 Xp);

    // --- Health --- //

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float Health = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    UAnimMontage* DeathMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regen")
    float TimeBeforeRegenStarts = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regen")
    float RegenAmount = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regen")
    float RegenInterval = 0.5f;

    FTimerHandle RegenStartTimer;
    FTimerHandle RegenTickTimer;

    virtual float TakeDamage(
        float DamageAmount,
        struct FDamageEvent const& DamageEvent,
        AController* EventInstigator,
        AActor* DamageCauser) override;

    UFUNCTION()
    void StartHealthRegen();

    UFUNCTION()
    void RegenHealth();

    // --- UI --- //

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UBUIUWCharacterHUD> HUDWidgetClass;

    UBUIUWCharacterHUD* HUDWidgetInstance;


    UPROPERTY(EditAnywhere, Category = "Game Over Screen")
    TObjectPtr <UUserWidget> GameOverScreen;

    // --- Music --- //

    enum playerMusicState {
        CALM = 1,
        HURT = 2,
        DYING = 3,
        DEAD = 4
    };

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
    TObjectPtr<UAkStateValue> CalmState;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
    TObjectPtr<UAkStateValue> HurtState;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
    TObjectPtr<UAkStateValue> DyingState;

    playerMusicState backgroundMusicPlayerState = playerMusicState::CALM;
};