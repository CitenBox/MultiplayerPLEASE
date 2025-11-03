// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerPLEASE/EGTypes.h"
#include "Engine/DataAsset.h"

#include "ItemBase.generated.h"

class UNiagaraSystem;

UCLASS(Blueprintable)
class MULTIPLAYERPLEASE_API UItemDataAsset : public USceneComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings")
	EItemType EquippedItemType = EItemType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings")
	TSoftObjectPtr<USkeletalMeshComponent> ItemMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings")
	TSoftObjectPtr<USkeletalMeshComponent> ItemMeshFP = nullptr;
	
};

UCLASS()
class MULTIPLAYERPLEASE_API UItemBase : public UObject
{
	GENERATED_BODY()

	
	public:
	
	

	 virtual ~UItemBase() override;

	UPROPERTY()
	ACharacter* OuterCharacterRef = nullptr;

	UPROPERTY()
	APlayerController* OuterPlayerControllerRef = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings")
	USkeletalMesh* ItemMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings")
	USkeletalMesh* ItemMeshFP = nullptr;
	
	TWeakPtr<FEGItemInfo> ItemInfoPtr = nullptr;
                                                 
	UPROPERTY()
	USkeletalMeshComponent* ItemMeshInstance = nullptr;
                                                 
	UPROPERTY()
	USkeletalMeshComponent* ItemMeshFPInstance = nullptr;
	
	virtual void Initialize(FEGItemInfo& ItemInfo);
	virtual void Initialize(FEGItemInfo& ItemInfo, ACharacter* OuterCharacter,
		APlayerController* OuterPlayerController);

	virtual void DoneInitiliaze(){}
	
	virtual void Action1(bool bActivate);

	virtual void Action2(bool bActivate);

	virtual void DestroyItem();
	
};
