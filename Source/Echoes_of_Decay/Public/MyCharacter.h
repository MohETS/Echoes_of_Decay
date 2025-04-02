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

protected:
    virtual void BeginPlay() override;

public:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// --- COMBAT --- //

	// Projectile class to spawn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TSubclassOf<class AProjectile> ProjectileClass;

	// Arrow component to specify the spawn location of the projectile
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    class UArrowComponent* MyArrowComponent;

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

    UPROPERTY(VisibleAnywhere, Category = "Inventory")
    UInventoryComponent* Inventory;

    UFUNCTION()
    void ToggleInventory();

	// --- WEAPON --- //

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TArray<TSubclassOf<AWeaponBase>> EquippedWeapons;

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

    // --- Health --- //

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float Health = 100.0f;

    virtual float TakeDamage(
        float DamageAmount,
        struct FDamageEvent const& DamageEvent,
        AController* EventInstigator,
        AActor* DamageCauser) override;

    // --- UI --- //

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UBUIUWCharacterHUD> HUDWidgetClass;

    UBUIUWCharacterHUD* HUDWidgetInstance;
};