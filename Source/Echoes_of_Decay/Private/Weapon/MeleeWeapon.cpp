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
   AttackDamage = 0.0f;  
}  

void AMeleeWeapon::Attack()  
{  
   UE_LOG(LogTemp, Warning, TEXT("Melee Weapon %s used attack!"), *WeaponName.ToString());  
   FVector Start = Owner->GetActorLocation();  
   FVector End = Start + Owner->GetActorForwardVector() * AttackRange;  

   FHitResult HitResult;  
   FCollisionQueryParams Params;  
   Params.AddIgnoredActor(Owner);  

   if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, Params))  
   {  
       AActor* HitActor = HitResult.GetActor();  
       if (HitActor)  
       {  
           UE_LOG(LogTemp, Warning, TEXT("Hit %s!"), *HitActor->GetName());  
           UGameplayStatics::ApplyDamage(HitActor, AttackDamage, Owner->GetInstigatorController(), this, nullptr);  

           if (WeaponEffect)  
           {  
               WeaponEffect->ApplyEffect(HitActor, Owner);  
           }  
       }  
   }  
}