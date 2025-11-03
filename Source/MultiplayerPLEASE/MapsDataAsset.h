// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MultiplayerPLEASE/EGTypes.h"

#include "MapsDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FMapInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> Map = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name = "None";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;

	
};

UCLASS(Blueprintable)
class MULTIPLAYERPLEASE_API UMapsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maps", meta=(TitleProperty = Name))
	TMap<int32, FMapInfo> Maps;

#if WITH_EDITOR
virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
	
};
