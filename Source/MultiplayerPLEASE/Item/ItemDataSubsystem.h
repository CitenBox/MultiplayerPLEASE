// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "MultiplayerPLEASE/EGTypes.h"
#include "ItemDataSubsystem.generated.h"

class UItemInfoDataAsset;

UCLASS(Blueprintable, BlueprintType)
class MULTIPLAYERPLEASE_API UItemDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	//friend class UItemInfoDataAsset;

public:
	
	UPROPERTY()
	TArray<FEGItemInfo> ItemInfoArray;



protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	UItemInfoDataAsset* ItemInfoDataAsset = nullptr;
};
