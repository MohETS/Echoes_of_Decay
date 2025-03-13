// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BUIUserWidget.h"
#include "BUIButton.h"
#include "BUILabel.h"
#include <Components/Image.h>
#include "BUIUWTitleScreenButton.generated.h"

/**
 * 
 */
UCLASS()
class UI_API UBUIUWTitleScreenButton : public UBUIUserWidget
{
	GENERATED_BODY()

public:

	/* UI Components */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UBUIButton> MainButton;

protected:

	/* UserWidget Functions */
	virtual void NativeConstruct() override;

	virtual void SynchronizeProperties() override;


	/* UI Components */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBUILabel> ButtonLabel;

	UPROPERTY(EditAnywhere)
	FText ButtonLabelText;

	/*UPROPERTY(EditAnywhere)
	bool bIsToggleButton;

	UPROPERTY(EditAnywhere)
	bool bIsToggleOn;*/
};