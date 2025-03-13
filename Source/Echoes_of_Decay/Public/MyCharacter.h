#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Projectile.h"
#include "MyCharacter.generated.h"


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

    // Fonction pour tirer un projectile
    void FireProjectile();

 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TSubclassOf<class AProjectile> ProjectileClass;

    // Emplacement du spawn du projectile
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    class UArrowComponent* MyArrowComponent;  
};
