// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"


#include "MPGameInstance.generated.h"

class UItemInfoDataAsset;
struct FEGItemInfo;

UCLASS()
class MULTIPLAYERPLEASE_API UMPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	UMPGameInstance();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Seed = 0;

	UPROPERTY(VisibleAnywhere, Category = "GlobalData", meta=(TitleProperty = Name))
	TArray<FEGItemInfo> ItemInfoDefinitionsArray;

//data asset of item data
	
	UPROPERTY(EditAnywhere, Category = "DataAsset")
	UItemInfoDataAsset* ItemInfoDataAsset;

	
	virtual void Init() override;
	
};
