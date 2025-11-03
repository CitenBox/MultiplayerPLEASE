// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "UObject/Class.h"

#include "EGTypes.generated.h"

//this is used to decide which item animation to play
UENUM(BlueprintType)
enum class EItemType : uint8
{
	None UMETA(DisplayName="None"),
	Rifle UMETA(DisplayName="Rifle"),
};


//this enum is defines flags that can be added to items if for example an item is tradable or not or stuff like that (for now isn't being used and is more of a maybe)
UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EEGItem : int32
{
	None = 0x0,
	Null = 0x1,
	Tradeable = 0x2,
	Apple = 0x4,
	GreenApple = 0x8,
	BlueApple = 0x10,
	RedApple = 0x20,
};
ENUM_CLASS_FLAGS(EEGItem)

//todo: look into FTR. and networking compression, quantization and custom struct serialization. and regular serialization for networking.
//This struct is stores both static item data and instance item data so an inventory will contain an array of these. the static
//information is marked as not replicated to optimize it.
USTRUCT()
struct FEGItemInfo : public FFastArraySerializerItem
{
	GENERATED_BODY()

	//index of -1 means invalid
	UPROPERTY()
	int16 Index = -1;

	//ID of 0 means invalid.
	//id used to identify what item you have in a lot of places and 
	UPROPERTY(EditAnywhere)
	uint16 ID = 0;

	//for now isn't being used by anything but will definately could be useful for a lot of stuff.
	UPROPERTY(EditAnywhere, NotReplicated, meta = (Bitmask, BitmaskEnum = EEGItem))
	int32 ItemFlags;
	
	UPROPERTY(EditAnywhere, NotReplicated)
	EItemType ItemType = EItemType::None;

	UPROPERTY(EditAnywhere, NotReplicated)
	int32 MaxStack = 1;

	UPROPERTY(EditAnywhere)
	int32 CurrentStack = 1;

	UPROPERTY(EditAnywhere)
	int32 Data1 = 0;

	UPROPERTY(EditAnywhere)
	int32 Data2 = 0;

	UPROPERTY(EditAnywhere)
	int32 Value = 0;

	UPROPERTY(EditAnywhere, NotReplicated)
	FString Name = TEXT("None");

	UPROPERTY(EditAnywhere, NotReplicated)
	FString Description = TEXT("None");

	UPROPERTY(EditAnywhere, NotReplicated)
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, NotReplicated)
	TSubclassOf<class UItemBase> ItemImplementation = nullptr;

	UPROPERTY(EditAnywhere, NotReplicated)
	TSubclassOf<class ABasePickup> ItemPickupClass = nullptr;
};

FORCEINLINE void SetNoneInstanceData(FEGItemInfo& Modify, const FEGItemInfo& Source)
{
	Modify.ID = Source.ID;
	Modify.Name = Source.Name;
	Modify.Description = Source.Description;
	Modify.Icon = Source.Icon;
	Modify.MaxStack = Source.MaxStack;
	Modify.ItemFlags = Source.ItemFlags;
}

union SignConvertion
{
	int32 Signed;
	uint32 UnSigned;
};