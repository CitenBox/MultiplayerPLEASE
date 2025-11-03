// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthUI.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHealthUI::UpdateHealthUI(float NewHealth)
{
	if(HealthProgressBar)
	{
		HealthProgressBar->SetPercent(NewHealth/MaxHealth);
	}

	if(HealthText)
	{
		HealthText->SetText(FText().FromString(FString::FromInt(static_cast<int32>(NewHealth))));
	}
	
}
