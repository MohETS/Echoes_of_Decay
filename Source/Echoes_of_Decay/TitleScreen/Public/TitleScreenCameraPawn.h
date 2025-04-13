// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TitleScreenCameraPawn.generated.h"

UCLASS()
class ECHOES_OF_DECAY_API ATitleScreenCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATitleScreenCameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere)
	TSubclassOf<class UBUIUWTitleScreen> TitleScreenClass;

	UPROPERTY()
	TObjectPtr<UBUIUWTitleScreen> TitleScreen;

};
