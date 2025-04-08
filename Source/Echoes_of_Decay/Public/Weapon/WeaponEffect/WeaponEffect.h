#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponEffect.generated.h"

UCLASS(EditInlineNew, BlueprintType)
class ECHOES_OF_DECAY_API UWeaponEffect : public UObject
{
	GENERATED_BODY()
	
public:
	// Constructor
	UWeaponEffect();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FName EffectName;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon Effect")
	void ApplyEffect(AActor* Target, AActor* Instigator);
};