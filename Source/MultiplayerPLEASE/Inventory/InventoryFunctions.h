// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../EGTypes.h"
#include "CoreMinimal.h"
#include "InventoryFunctions.generated.h"

UCLASS()
class UInventoryFunctions : public UObject
{
	GENERATED_BODY()

	public:

	static bool Stack(FEGItemInfo& Source, FEGItemInfo& Target);
};




