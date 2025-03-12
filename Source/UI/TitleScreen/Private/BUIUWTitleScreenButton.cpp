// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleScreen/Public/BUIUWTitleScreenButton.h"

void UBUIUWTitleScreenButton::NativeConstruct()
{
	Super::NativeConstruct();

}

void UBUIUWTitleScreenButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (ButtonLabel) {
		ButtonLabel->SetText(ButtonLabelText);
	}

}
