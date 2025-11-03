// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryFunctions.h"

bool UInventoryFunctions::Stack(FEGItemInfo& Source, FEGItemInfo& Target)
{
	if (Source.ID != Target.ID)
	{
		return false;
	}

	if (Target.CurrentStack + Source.CurrentStack >= Source.MaxStack)
	{
		Source.CurrentStack -= Target.MaxStack - Target.CurrentStack;
		Target.CurrentStack = Target.MaxStack;

		return (bool)Source.CurrentStack;
	}
	else
	{
		Target.CurrentStack += Source.CurrentStack;
		Source.CurrentStack = 0;

		return true;
	}
}
