// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthUI.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class MULTIPLAYERPLEASE_API UHealthUI : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta=(BindWidgetOptional))
	UProgressBar* HealthProgressBar;

	UPROPERTY(EditAnywhere, meta=(BindWidgetOptional))
	UTextBlock* HealthText;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;

	void UpdateHealthUI(float NewHealth);
	
};
