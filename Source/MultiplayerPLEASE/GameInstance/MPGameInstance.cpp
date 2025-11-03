// Fill out your copyright notice in the Description page of Project Settings.


#include "MPGameInstance.h"

#include "MultiplayerPLEASE/EGTypes.h"
#include "MultiplayerPLEASE/Item/ItemInfoDataAsset.h"


UMPGameInstance::UMPGameInstance()
{
}

void UMPGameInstance::Init()
{
	Super::Init();

	UItemInfoDataAsset::SyncItemInfoArray(ItemInfoDefinitionsArray, ItemInfoDataAsset);
}
