// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BUIUserWidget.h"
#include "BUIButton.h"
#include "BUILabel.h"
#include "BUIImage.h"
#include "BUIUWWeaponAbilityContainer.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBUIOnPressedSignature, class UBUIUWWeaponAbilityContainer*, Button);

/**
 *
 */
UCLASS()
class UI_API UBUIUWWeaponAbilityContainer : public UBUIUserWidget
{
	GENERATED_BODY()

public:

	// Bind a function with the signature "void OnPressed(UBUIUWWeaponAbilityContainer* Button);
	FBUIOnPressedSignature OnPressedDelegate;

	/* Functions */
	UFUNCTION()
	FText GetWeaponAbilityName();

	/* UI Components */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UBUIButton> WeaponAbility;

protected:

	/* UserWidget Functions */
	virtual void NativeConstruct() override;

	virtual void SynchronizeProperties() override;

	/* Functions */
	UFUNCTION()
	void OnButtonPressed();

	/* UI Components */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBUILabel> WeaponAbilityName;

	UPROPERTY(EditAnywhere)
	FText WeaponAbilityNameLabelText;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBUIImage> WeaponAbilityImage;


};