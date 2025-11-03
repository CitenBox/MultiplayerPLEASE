// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MultiplayerPLEASE/EGTypes.h"

#include "ItemInfoDataAsset.generated.h"

UCLASS()
class MULTIPLAYERPLEASE_API UItemInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UFUNCTION(CallInEditor)
	void ExportAsJson();

	UFUNCTION(CallInEditor)
	void ImportAsJson();

	UFUNCTION(CallInEditor)
	void SyncIdToIndex();
	

	static void SyncItemInfoArray(TArray<FEGItemInfo>& OutArray, const UItemInfoDataAsset* SourceDataAsset);
	
	UPROPERTY(EditAnywhere, Category = "JsonSettings")
	FString FilePath = "";

protected:
	
	UPROPERTY(EditAnywhere, Category = "ItemInfo", meta=(TitleProperty = Name))
	TArray<FEGItemInfo> ItemInfoArray;
	FEGItemInfo DefaultItemInfo;
	
public:

	const FEGItemInfo& GetItemInfo(uint16 Id);
	
	bool IsValidId(uint16 Id) const {return ItemInfoArray.IsValidIndex(Id - 1);}
	
};
