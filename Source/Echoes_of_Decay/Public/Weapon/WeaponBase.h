#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponEffect/WeaponEffect.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Melee UMETA(DisplayName = "Melee"),
	Ranged UMETA(DisplayName = "Ranged"),
	Buff UMETA(DisplayName = "Buff")
};

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
    FName WeaponName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 Level;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 XP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 XPToNextLevel;

    UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category = "Weapon")
    UWeaponEffect* WeaponEffect;

	ACharacter* Owner;

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void Attack();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void GainXP(int32 Amount);
};