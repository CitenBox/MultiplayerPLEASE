// Fill out your copyright notice in the Description page of Project Settings.
#include "Inventory.h"

#include "MultiplayerPLEASE/EGTypes.h"

#include "InventoryFunctions.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerPLEASE/GameInstance/MPGameInstance.h"
#include "Net/Core/PushModel/PushModel.h"
#include "MultiplayerPLEASE/Item/ItemDataSubsystem.h"
#include "MultiplayerPLEASE/Item/ItemInfoDataAsset.h"

void UInventory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UInventory, InventoryArray);
}

void UInventory::BeginPlay()
{
	Super::BeginPlay();

	InventoryArray.Items.SetNum(MaxItems);
	MarkArrayDirty();
	
	if(UWorld* World = GetWorld())
	{
		MPGameInstance = Cast<UMPGameInstance>(World->GetGameInstance());
		if(MPGameInstance)
		{
			ItemInfoDataAsset = MPGameInstance->ItemInfoDataAsset;
		}
	}
}

void UInventory::OnRep_InventoryArray()
{
	if(GetNetMode() == NM_DedicatedServer || !ItemInfoDataAsset)
	{
		return;
	}

	TArray<FEGItemInfo> Temp;
	Temp.SetNum(MaxItems);
	for(int32 i = 0; i < InventoryArray.Items.Num(); i++)
	{
		if(InventoryArray.Items.IsValidIndex(i))
		{
			if(Temp.IsValidIndex(InventoryArray.Items[i].Index))
			{
				Temp[InventoryArray.Items[i].Index] = InventoryArray.Items[i];
				SetNoneInstanceData(Temp[InventoryArray.Items[i].Index], ItemInfoDataAsset->GetItemInfo(InventoryArray.Items[i].ID));
			}
		}
	}

	InventoryArray.Items = Temp;
	
	if (OnInventoryUpdate.IsBound())
	{
		OnInventoryUpdate.Broadcast();
	}
}

void UInventory::MarkDirtyAt(int32 Index)
{
	InventoryArray.MarkItemDirty(InventoryArray.Items[Index]);
	OnRep_InventoryArray();
}

void UInventory::MarkArrayDirty()
{
	InventoryArray.MarkArrayDirty();
	OnRep_InventoryArray();
}

UInventory::UInventory()
{
}


UInventory* UInventory::operator=(UInventory* LhsInventory)
{
	if(IsValid(LhsInventory))
	{
		InventoryArray.Items = LhsInventory->InventoryArray.Items;
		MarkArrayDirty();
	}

	return this;
}

//returns the remainder and -1 if failed
int32 UInventory::AddItem(const FEGItemInfo& Item)
{
	FEGItemInfo ItemCopy = Item;
	const int32 OriginalStack = Item.CurrentStack;

	if(ItemCopy.ID == 0)
	{
		return -1;
	}
	
	for (int32 i = 0; i < InventoryArray.Items.Num(); i++)
	{
		if(InventoryArray.Items[i].ID == 0)
		{
			continue;
		}
		
		const int32 ItemCopyTempCurrentStack = ItemCopy.CurrentStack;
		Stack(ItemCopy, InventoryArray.Items[i]);

		if (ItemCopy.CurrentStack <= 0)
		{
			MarkDirtyAt(i);
			return 0;
		}

		if(ItemCopy.CurrentStack != ItemCopyTempCurrentStack)
		{
			MarkDirtyAt(i);
		}
	}

	for (int32 i = 0; i < InventoryArray.Items.Num(); i++)
	{
		if (InventoryArray.Items[i].ID == 0)
		{
			ItemCopy.Index = i;
			InventoryArray.Items[i] = ItemCopy;
			MarkDirtyAt(i);
			
			return 0;
		}
	}

	if (OriginalStack != ItemCopy.CurrentStack)
	{
		OnRep_InventoryArray();
	}
	
	return ItemCopy.CurrentStack;
}

void UInventory::AddItemSmart(FEGItemInfo& Item)
{
	const int32 OriginalStack = Item.CurrentStack;

	if(Item.ID == 0)
	{
		return;
	}
	
	for (int32 i = 0; i < InventoryArray.Items.Num(); i++)
	{
		if(InventoryArray.Items[i].ID == 0)
		{
			continue;
		}
		
		const int32 ItemCopyTempCurrentStack = Item.CurrentStack;
		Stack(Item, InventoryArray.Items[i]);

		if (Item.CurrentStack <= 0)
		{
			MarkDirtyAt(i);
			return;
		}

		if(Item.CurrentStack != ItemCopyTempCurrentStack)
		{
			MarkDirtyAt(i);
		}
	}

	for (int32 i = 0; i < InventoryArray.Items.Num(); i++)
	{
		if (InventoryArray.Items[i].ID == 0)
		{
			Item.Index = i;
			InventoryArray.Items[i] = Item;
			MarkDirtyAt(i);
			
			return;
		}
	}

	if (OriginalStack != Item.CurrentStack)
	{
		OnRep_InventoryArray();
	}
}

int32 UInventory::AddItemAt(const FEGItemInfo& Item, int32 Index)
{
	FEGItemInfo ItemCopy = Item;
	const int32 OriginalStack = Item.CurrentStack;
	
	if(InventoryArray.Items.IsValidIndex(Index))
	{
		Stack(ItemCopy, InventoryArray.Items[Index]);
        	
        
		if(ItemCopy.CurrentStack != OriginalStack)
		{
			MarkDirtyAt(Index);
			return ItemCopy.CurrentStack;
		}

		if(InventoryArray.Items[Index].ID == 0)
		{
			ItemCopy.Index = Index;
			InventoryArray.Items[Index] = ItemCopy;
			MarkDirtyAt(Index);
			return 0;
		}

		return ItemCopy.CurrentStack;
	}
	
	return ItemCopy.CurrentStack;
}

void UInventory::AddItemAtSmart(FEGItemInfo& Item, int32 Index)
{
	if(InventoryArray.Items.IsValidIndex(Index))
	{
		if(InventoryArray.Items[Index].ID == 0)
		{
			FEGItemInfo temp = Item;
			temp.Index = Index;
			InventoryArray.Items[Index] = temp;
			SetItemInvalid(Item);
			
			MarkDirtyAt(Index);
			return;
		}

		const int32 OriginalStack = Item.CurrentStack;
		Stack(Item, InventoryArray.Items[Index]);
		
		if(Item.CurrentStack != OriginalStack)
		{
			MarkDirtyAt(Index);
		}
	}
}

bool UInventory::MoveItem(int32 SourceIndex, int32 DestinationIndex)
{
	if(SourceIndex == DestinationIndex)
	{
		return false;
	}
	
	if(!IsValidItem(SourceIndex))
	{
		return false;
	}
	
	AddItemAtSmart(InventoryArray.Items[SourceIndex], DestinationIndex);

	if(InventoryArray.Items[SourceIndex].ID == 0)
	{
		return true;
	}

	return false;
}

void UInventory::MoveItemServerRPC_Implementation(int32 SourceIndex, int32 DestinationIndex)
{
	MoveItem(SourceIndex, DestinationIndex);
}

bool UInventory::IsValidIndex(int32 Index) const
{
	return InventoryArray.Items.IsValidIndex(Index); ;
}

bool UInventory::IsValidItem(int32 Index) const
{
	return InventoryArray.Items.IsValidIndex(Index) && InventoryArray.Items[Index].ID != 0;
}

//doesnt actually remove an array element but sets it's ID to -1 and replicates it
bool UInventory::RemoveAt(int32 Index)
{
	if(InventoryArray.Items.IsValidIndex(Index) && InventoryArray.Items[Index].ID != 0)
	{
		InventoryArray.Items[Index].ID = 0;
		MarkDirtyAt(Index);
		return true;
	}

	return false;
}

int32 UInventory::RemoveStackable(const FEGItemInfo& ItemToRemove)
{
	int32 ItemToRemoveStack = ItemToRemove.CurrentStack;
	
	for(FEGItemInfo& E : InventoryArray.Items)
	{
		if(E.ID != ItemToRemove.ID)
		{
			continue;
		}

		const int32 TempStack = ItemToRemoveStack;
		ItemToRemoveStack -= E.CurrentStack;
		E.CurrentStack -= TempStack;

		if(E.CurrentStack <= 0)
		{
			E.ID = 0;
		}
		
		InventoryArray.MarkItemDirty(E);

		if(ItemToRemoveStack <= 0)
		{
			OnRep_InventoryArray();
			return 0;
		}
	}

	if(ItemToRemoveStack != ItemToRemove.CurrentStack)
	{
		OnRep_InventoryArray();
		return ItemToRemoveStack;
	}

	return ItemToRemoveStack;
}

int32 UInventory::Remove(const FEGItemInfo& ItemToRemove)
{
	return RemoveStackable(ItemToRemove);

	//for now i will just use remove stackable since it
	//works for both stackable and none stackable items and
	// i suspect it might be faster.
/*	
	if(ItemToRemove.ID == -1) {return -1;}
	if(ItemToRemove.CurrentStack <= 0) {return ItemToRemove.CurrentStack;}

	
	if(ItemToRemove.MaxStack == 1)
	{
		for(FEGItemInfo& E : InventoryArray.Items)
		{
			if(E.ID == ItemToRemove.ID)
			{
				E.ID = -1;
				InventoryArray.MarkItemDirty(E);
				OnRep_InventoryArray();
				return 0;
			}
		}

		return 1;
	}

	return RemoveStackable(ItemToRemove);
	*/
}


const FEGItemInfo& UInventory::GetConstRefAt(int32 Index)
{
	if(InventoryArray.Items.IsValidIndex(Index))
	{
		return InventoryArray.Items[Index];
	}

	static FEGItemInfo Temp = FEGItemInfo();
	return Temp;
}

FEGItemInfo UInventory::GetCopyAt(int32 Index)
{
	return GetConstRefAt(Index);
}

//return true if there was a change (to be used to mark dirty)
bool UInventory::Stack(FEGItemInfo& Source, FEGItemInfo& Target)
{
	if (Source.ID != Target.ID || Target.ID == 0)
	{
		return false;
	}

	if (Source.CurrentStack == Source.MaxStack)
	{
		return false;
	}
	
	if (Target.CurrentStack + Source.CurrentStack >= Source.MaxStack)
	{
		Source.CurrentStack -= Target.MaxStack - Target.CurrentStack;
		Target.CurrentStack = Target.MaxStack;

		return true;
	}

	Target.CurrentStack += Source.CurrentStack;
	Source.CurrentStack = 0;

	return true;
}

void UInventory::SetItemInvalid(FEGItemInfo& Item)
{
	Item.ID = 0;
	Item.Index = -1;
}
