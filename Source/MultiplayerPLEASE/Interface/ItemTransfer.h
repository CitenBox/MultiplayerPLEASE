// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MultiplayerPLEASE/EGTypes.h"

#include "ItemTransfer.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemTransfer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTIPLAYERPLEASE_API IItemTransfer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//returns remainder of stack if stackable and 0 if successfully given and -1 if it failed
	virtual int32 GiveItem(const FEGItemInfo& Item);

	//Takes an item at an index NOTE: if amount is larger than the item's current stack it will just take what ever is there
	virtual FEGItemInfo TakeItemAt(int32 Index, int32 Amount);

	//takes an item that matches type and from specified amount
	virtual FEGItemInfo TakeItem(int32 ID, int32 Amount);

	
};
