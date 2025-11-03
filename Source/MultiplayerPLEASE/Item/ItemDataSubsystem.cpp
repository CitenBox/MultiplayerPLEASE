// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataSubsystem.h"

#include "ItemInfoDataAsset.h"

void UItemDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	if(ItemInfoDataAsset)
	{
		if(UGameInstance* GameInstance = GetGameInstance())
		{
			UItemDataSubsystem* ItemDataSubsystem = GameInstance->GetSubsystem<UItemDataSubsystem>();
			ItemDataSubsystem->ItemInfoDataAsset = ItemInfoDataAsset;
			if(ItemInfoDataAsset && ItemDataSubsystem)
			{
				//ItemDataSubsystem->ItemInfoArray = ItemInfoDataAsset->ItemInfoArray;
			}
		}

	}
}
