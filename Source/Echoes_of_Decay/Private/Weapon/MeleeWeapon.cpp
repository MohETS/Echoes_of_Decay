#include "Weapon/MeleeWeapon.h"  
#include "GameFramework/Actor.h"  
#include "Kismet/KismetSystemLibrary.h"  
#include "Kismet/GameplayStatics.h"  
#include "GameFramework/Character.h"

AMeleeWeapon::AMeleeWeapon()  
{  
   WeaponType = EWeaponType::Melee;  
   WeaponName = "Sword";  
   AttackRange = 200.0f;  
   AttackDamage = 20.0f;  
}  

void AMeleeWeapon::Attack()  
{  
    if (!bCanAttack) return;

    FVector Start = Owner->GetActorLocation();  
    FVector End = Start + Owner->GetActorForwardVector() * AttackRange;  

    FHitResult HitResult;  
    FCollisionQueryParams Params;  
    Params.AddIgnoredActor(Owner);  

    if (!GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, Params))  return;

    AActor* HitActor = HitResult.GetActor();  
    if (!HitActor) return;

    UGameplayStatics::ApplyDamage(HitActor, AttackDamage, Owner->GetInstigatorController(), this, nullptr);  
    GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AMeleeWeapon::ResetAttackCooldown, AttackCooldown, false);

    if (!WeaponEffect) return;
    WeaponEffect->ApplyEffect(HitActor, Owner);  
}

void AMeleeWeapon::ApplyWeaponLevelEffects()
{
    AttackDamage = AttackDamage * 1.1;
}
