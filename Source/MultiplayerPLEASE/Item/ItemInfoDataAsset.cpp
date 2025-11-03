// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfoDataAsset.h"

#include "JsonObjectConverter.h"
#include "HAL/PlatformFileManager.h"
#include "Serialization/JsonSerializer.h"
#include "ItemDataSubsystem.h"

void UItemInfoDataAsset::ExportAsJson()
{
	/*FString JsonString;

	if (!FJsonObjectConverter::UStructToJsonObjectString<FItemInfoArray>(ItemInfoArray, JsonString))
	{
		UE_LOG(LogTemp, Error, TEXT("Writing to file %s failed - is it a text or json file?"), *FilePath);
		return;
	}
	

	if (!FFileHelper::SaveStringToFile(JsonString, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Writing to file %s failed"), *FilePath);
	}*/
}

void UItemInfoDataAsset::ImportAsJson()
{
	/*if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT(" file: %s. Doesn't exist"), *FilePath);
		return;
	}

	FString JsonString = "";

	if(!FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("file: %s. Is not a text or a json file"), *FilePath);
		return;
	}
	
	if(!FJsonObjectConverter::JsonObjectStringToUStruct<FItemInfoArray>(JsonString ,&ItemInfoArray))
	{
		UE_LOG(LogTemp, Error, TEXT("Json convertion failed - are you sure that the field is valid?"), *JsonString);
	}*/
}

void UItemInfoDataAsset::SyncIdToIndex()
{
	for(int32 i = 0; i < ItemInfoArray.Num(); ++i)
	{
		if(ItemInfoArray.IsValidIndex(i))
		{
			ItemInfoArray[i].ID = i + 1;
		}
	}
}

void UItemInfoDataAsset::SyncItemInfoArray(TArray<FEGItemInfo>& OutArray, const UItemInfoDataAsset* SourceDataAsset)
{
if(SourceDataAsset)
{
	if(SourceDataAsset->ItemInfoArray.Num() == 0)
	{
		return;
	}
		
	OutArray.Reset(SourceDataAsset->ItemInfoArray.Num());
	OutArray.SetNumZeroed(SourceDataAsset->ItemInfoArray.Num());

	for(int32 i = 0; i < OutArray.Num(); i++)
	{
		OutArray[i] = SourceDataAsset->ItemInfoArray[i];
	}
}
}

const FEGItemInfo& UItemInfoDataAsset::GetItemInfo(uint16 Id)
{
	if(ItemInfoArray.IsValidIndex(Id - 1))
	{
		return ItemInfoArray[Id - 1];
	}

	return DefaultItemInfo;
}





