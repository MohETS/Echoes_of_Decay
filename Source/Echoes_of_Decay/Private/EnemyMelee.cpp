// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMelee.h"
#include "Kismet/GameplayStatics.h"

void AEnemyMelee::AttackPlayer()
{
    UE_LOG(LogTemp, Warning, TEXT("Melee Enemy Attacking!"));

    // Appliquer des dégâts au joueur
    if (PlayerPawn)
    {
        ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerPawn);
        if (PlayerCharacter)
        {
            UGameplayStatics::ApplyDamage(PlayerCharacter, 20.0f, GetController(), this, UDamageType::StaticClass());
        }
    }
}