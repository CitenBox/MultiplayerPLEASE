// Fill out your copyright notice in the Description page of Project Settings.


#include "MapsDataAsset.h"

#include "JsonObjectConverter.h"
#include "HAL/PlatformFileManager.h"
#include "Serialization/JsonSerializer.h"

#if WITH_EDITOR
void UMapsDataAsset::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	for(auto& Map : Maps)
	{
		Map.Value.ID = Map.Key;
	}
}
#endif
