#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponEffect/WeaponEffect.h"
#include "WeaponType.h"
#include "Inventory/ItemType.h"
#include "WeaponBase.generated.h"


UCLASS(Blueprintable, BlueprintType)
class ECHOES_OF_DECAY_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    EWeaponType WeaponType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 WeaponLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 MaxWeaponLevel = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 XP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 XPToNextLevel;

    UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category = "Weapon")
    UWeaponEffect* WeaponEffect;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bCanAttack = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    float AttackCooldown = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryUI")
    FName WeaponName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryUI")
    UTexture2D* WeaponIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryUI")
    EItemType ItemType = EItemType::Weapon;

	ACharacter* Owner;

    FTimerHandle AttackTimer;

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void Attack();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void GainXP(int32 Amount);

    void ResetAttackCooldown();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void LevelUp();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void ApplyWeaponLevelEffects();
};