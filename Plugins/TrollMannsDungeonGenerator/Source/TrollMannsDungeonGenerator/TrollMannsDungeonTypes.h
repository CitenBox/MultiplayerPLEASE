// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrollMannsDungeonTypes.generated.h"

/*
template<typename InElementType>
class TMatrixArray : public TArray<InElementType>
{
protected:

	TArray<int32> DimensionsArray;
	
	TArray<InElementType> Data;

	template<typename Dimension, typename... DimensionArgs>
	int32 CalcAbsIndex(int32 Count, Dimension CurrentDimension, DimensionArgs...Args)
	{
		return CurrentDimension + DimensionsArray[Count] * CalcAbsIndex(Count + 1, Args...);
	}

	template<typename Dimension>
	int32 CalcAbsIndex(int32 Count, Dimension CurrentDimension)
	{
		return CurrentDimension;
	}
public:
	TMatrixArray(const TArray<int32>& DimensionsArray)
	{
		ensureMsgf(DimensionsArray.Num() >= 2, TEXT("Not enough dimensions. consider using a normal array instead"));
		this->DimensionsArray = DimensionsArray;

		int32 FinalNum = DimensionsArray[0];
		for(int32 i = 0; i < DimensionsArray.Num() - 1; ++i)
		{
			FinalNum *= DimensionsArray[i + 1];
		}
		Data.SetNum(FinalNum);
	};

	TMatrixArray(const TMatrixArray<InElementType>& other) :
	Data {other.Data},
	DimensionsArray {other.DimensionsArray} {}

	InElementType& operator	[] (int32 Index) {return Data[Index];}
	
	template<typename Dimension, typename... DimensionArgs>
	InElementType& Get(Dimension FirstDimension, DimensionArgs...Args)
	{
		return Data[CalcAbsIndex(0, FirstDimension, Args...)];
	}
	
	
};
*/

USTRUCT()
struct TROLLMANNSDUNGEONGENERATOR_API F3DMatrixLayout
{
	GENERATED_BODY()

	TArray<int32> Array;
	
	uint8 SizeX = 1;
	uint8 SizeY = 1;
	uint8 SizeZ = 1;

	F3DMatrixLayout(){};
	F3DMatrixLayout(uint8 SizeX, uint8 SizeY, uint8 SizeZ);
	F3DMatrixLayout(const F3DMatrixLayout& Other);
	
	int32& Get(int32 X, int32 Y, int32 Z);
	
};

UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class  EConnection : int32
{
	NONE = 0x00 UMETA(DisplayName = "NONE"),
	FORWARD = 0x01 UMETA(DisplayName = "FORWARD"),
	BACKWARD = 0x02 UMETA(DisplayName = "BACKWARD"),
	RIGHT = 0x04 UMETA(DisplayName = "RIGHT"),
	LEFT = 0x08 UMETA(DisplayName = "LEFT"),
	VISITED = 0x10 UMETA(DisplayName = "VISITED"),
	UP = 0x20 UMETA(DisplayName = "UP"),
	DOWN = 0x40 UMETA(DisplayName = "DOWN"),
	SPAWNED = 0x80 UMETA(DisplayName = "SPAWNED")
};
ENUM_CLASS_FLAGS(EConnection);

USTRUCT()
struct TROLLMANNSDUNGEONGENERATOR_API FRoomComponentData
{
	GENERATED_BODY()

	FRoomComponentData();

	UPROPERTY(EditAnywhere)
	FTransform Transform;

	UPROPERTY(EditAnywhere)
	UStaticMesh* StaticMesh = nullptr;
};

UCLASS()
class TROLLMANNSDUNGEONGENERATOR_API UDungeonRoomDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Special Room Settings")
	bool bIsSpecialRoom = false;

	UPROPERTY(EditAnywhere, Category = "Special Room Settings", meta = (ClampMin = 1, EditCondition = bIsSpecialRoom))
	int32 Height = 2;

	UPROPERTY(EditAnywhere, Category = "Special Room Settings", meta = (ClampMin = 1, EditCondition = bIsSpecialRoom))
	int32 Width = 2;

	UPROPERTY(EditAnywhere, Category = "Special Room Settings", meta = (ClampMin = 1, EditCondition = bIsSpecialRoom))
	int32 Length = 2;

	//index to connection flag map
	UPROPERTY(EditAnywhere, Category = "Special Room Settings", meta = (ClampMin = 1, EditCondition = bIsSpecialRoom))
	TMap<uint32, int32> ConnectionFlagsMap;
	
	UPROPERTY(EditAnywhere,  Category = "Room Settings")
	TArray<FRoomComponentData> RoomMeshInfoArray;

	UPROPERTY(EditAnywhere, Category = "Room Settings", meta = (ClampMin = 1))
	int32 RoomSize = 1000;
                                                  
	UPROPERTY(EditAnywhere,  Category = "Room Settings", meta = (Bitmask, BitmaskEnum = EConnection))
	int32 ConnectionFlags = (int32)EConnection::NONE;
};

UCLASS(BlueprintType, Blueprintable)
  class TROLLMANNSDUNGEONGENERATOR_API UDungeonRoomsDataAsset : public UDataAsset
  {
  	GENERATED_BODY()
  
  public:

	void FindRoomsWithFlags(TArray<UDungeonRoomDataAsset*>& OutResult, int32 Flags = 0) const;
  
  	UPROPERTY(EditAnywhere)
  	TMap<UDungeonRoomDataAsset*, TSubclassOf<class ADungeonRoom>> RoomTypes;
  };