// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRanged.h"
#include "Kismet/GameplayStatics.h"


void AEnemyRanged::AttackPlayer()
{
    UE_LOG(LogTemp, Warning, TEXT("Ranged Enemy Shooting!"));

    if (ProjectileClass && PlayerPawn)
    {
        FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
        FRotator SpawnRotation = (PlayerPawn->GetActorLocation() - GetActorLocation()).Rotation();

        GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
    }
}