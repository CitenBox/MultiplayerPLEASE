// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemTransfer.h"

// Add default functionality here for any IItemTransfer functions that are not pure virtual.
int32 IItemTransfer::GiveItem(const FEGItemInfo& Item)
{
	return Item.CurrentStack;
}

FEGItemInfo IItemTransfer::TakeItemAt(int32 Index, int32 Amount)
{
	return FEGItemInfo();
}

FEGItemInfo IItemTransfer::TakeItem(int32 ID, int32 Amount)
{
	return FEGItemInfo();
}
