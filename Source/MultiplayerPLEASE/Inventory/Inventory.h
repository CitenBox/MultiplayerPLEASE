// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MultiplayerPLEASE/EGTypes.h"
#include "Inventory.generated.h"

class UItemInfoDataAsset;
class UItemDataSubsystem;
class UMPGameInstance;


USTRUCT()
struct FInventoryArray : public FFastArraySerializer
{
	GENERATED_BODY()
	UPROPERTY()
	TArray<FEGItemInfo> Items;
	bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParms)
	{
		return FastArrayDeltaSerialize<FEGItemInfo>( Items, DeltaParms, *this);
	}
};

template<>
struct TStructOpsTypeTraits<FInventoryArray> : TStructOpsTypeTraitsBase2<FInventoryArray>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdate);

UCLASS(Blueprintable)
class MULTIPLAYERPLEASE_API UInventory : public UActorComponent
{
	GENERATED_BODY()

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	

public:
//deal with relevancy conundrum
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_InventoryArray)
	FInventoryArray InventoryArray;
	
	UPROPERTY(EditAnywhere)
	int32 MaxItems = 20;

	UPROPERTY(EditAnywhere)
	int32 HotbarSlotNum = 6;

	//general static data
	UPROPERTY()
	UMPGameInstance* MPGameInstance;

	//item system static data
	UPROPERTY()
	UItemInfoDataAsset* ItemInfoDataAsset = nullptr;
	
	UInventory();

	//if valid it means that the inventory is owned by a character
	UPROPERTY()
	ACharacter* OwningCharacter = nullptr;

	UInventory* operator=(UInventory* LhsInventory);
	
	int32 AddItem(const FEGItemInfo& Item);
	
	void AddItemSmart(FEGItemInfo& Item);
	
	int32 AddItemAt(const FEGItemInfo& Item, int32 Index);
	
	void AddItemAtSmart(FEGItemInfo& Item, int32 Index);
	
	bool MoveItem(int32 SourceIndex, int32 DestinationIndex);

	UFUNCTION(Server, Reliable)
	void MoveItemServerRPC(int32 SourceIndex, int32 DestinationIndex);

	//better to use IsValidItem instead since this just checks if there is an array element,
	//and not if the item in it is valid or not.
	bool IsValidIndex(int32 Index) const;
	//checks if there is an actual item at the specified index, instead of some garbage.
	bool IsValidItem(int32 Index) const;

	//doesnt actually remove an array element but sets it's ID to -1 and replicates it
	UFUNCTION()
	bool RemoveAt(int32 Index);

	int32 RemoveStackable(const FEGItemInfo& ItemToRemove);
	
	int32 Remove(const FEGItemInfo& ItemToRemove);

	UFUNCTION()
	const FEGItemInfo& GetConstRefAt(int32 Index);

	UFUNCTION()
	FEGItemInfo GetCopyAt(int32 Index);
	


	UFUNCTION()
	void OnRep_InventoryArray();

	void MarkDirtyAt(int32 Index);

	void MarkArrayDirty();
	
	FOnInventoryUpdate OnInventoryUpdate;

protected:
	
	bool Stack(FEGItemInfo& Source, FEGItemInfo& Target);

	void SetItemInvalid(FEGItemInfo& Item);
};
