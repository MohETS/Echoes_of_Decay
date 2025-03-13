// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BUIHUD.h"
#include "BUIUWCharacterHUD.h"
#include "BUIHUDMainPlayer.generated.h"

/**
 * 
 */
UCLASS()
class UI_API ABUIHUDMainPlayer : public ABUIHUD
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	ABUIHUDMainPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UBUIUWCharacterHUD> CharacterHUDClass;

	UPROPERTY()
	TObjectPtr<UBUIUWCharacterHUD> CharacterHUD;
};
