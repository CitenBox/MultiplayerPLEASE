// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotUI.h"

void UInventorySlotUI::OnButtonPressed()
{
	if(OnSlotPressedDelegate.IsBound())
	{
		OnSlotPressedDelegate.Execute(this, SlotIndex);
	}
}

void UInventorySlotUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemSlotButton)
	{
		ItemSlotButton->OnPressed.AddDynamic(this, &UInventorySlotUI::OnButtonPressed);
	}
}

