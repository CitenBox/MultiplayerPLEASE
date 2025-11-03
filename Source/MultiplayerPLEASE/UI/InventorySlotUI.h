// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WrapBoxSlot.h"


#include "InventorySlotUI.generated.h"

DECLARE_DELEGATE_TwoParams(FOnSlotPressed, UObject*, int32);
	
UCLASS()
class MULTIPLAYERPLEASE_API UInventorySlotUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
    void OnButtonPressed();

	void NativeConstruct() override;;
	
	FOnSlotPressed OnSlotPressedDelegate;
	
	UPROPERTY()
	int32 SlotIndex = -1;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ItemSlotButton;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* ItemSlotText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* ItemSlotImage;
	
};
