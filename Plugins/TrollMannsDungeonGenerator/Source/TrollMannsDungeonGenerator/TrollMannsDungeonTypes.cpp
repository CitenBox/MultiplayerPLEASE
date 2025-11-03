// Fill out your copyright notice in the Description page of Project Settings.

#include "TrollMannsDungeonTypes.h"

F3DMatrixLayout::F3DMatrixLayout(uint8 SizeX, uint8 SizeY, uint8 SizeZ) :
SizeX(SizeX), SizeY(SizeY), SizeZ(SizeZ)
{
	Array.SetNum(SizeX * SizeY * SizeZ);
}

F3DMatrixLayout::F3DMatrixLayout(const F3DMatrixLayout& Other):
Array(Other.Array),
SizeX(Other.SizeX),
SizeY(Other.SizeY),
SizeZ(Other.SizeZ)
{
}

int32& F3DMatrixLayout::Get(int32 X, int32 Y, int32 Z)
{
	return Array[X + SizeX * Y + SizeX * SizeY * Z];
}

FRoomComponentData::FRoomComponentData()
{
}

void UDungeonRoomsDataAsset::FindRoomsWithFlags(TArray<UDungeonRoomDataAsset*>& OutResult, int32 Flags) const
{
	TArray<UDungeonRoomDataAsset*> KeysArr;
	RoomTypes.GetKeys( KeysArr);
	for(UDungeonRoomDataAsset* RoomType : KeysArr)
	{
		if(RoomType)
		{
			if((RoomType->ConnectionFlags & Flags) == Flags)
			{
				OutResult.Add(RoomType);
			}
		}
	}
}
