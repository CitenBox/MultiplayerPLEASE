// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataAsset.h"
#include "TrollMannsDungeonTypes.h"
#include "DungeonRoom.generated.h"
//to mark a section as a room entrance create a scene component and add a tag to it that matches the RoomEntranceTag variable and move
//it to the place you want to be considered an entrance to the room.
UCLASS()
class TROLLMANNSDUNGEONGENERATOR_API ADungeonRoom : public AActor
{
	GENERATED_BODY()
	
public:
	
	ADungeonRoom();

	UPROPERTY(EditAnywhere, Category="Room Settings")
	UDungeonRoomDataAsset* RoomDataAsset = nullptr;

	UFUNCTION(CallInEditor, Category="Room Settings")
	void SyncData();

	UPROPERTY(EditAnywhere, Category="Room Settings")
	FName RoomEntranceTag = TEXT("Entrance");

	UPROPERTY(EditAnywhere, Category = "Special Room Settings")
	bool bIsSpecialRoom = false;

	UPROPERTY(EditAnywhere, Category = "Special Room Settings", meta = (ClampMin = 1, EditCondition = bIsSpecialRoom))
	int32 Height = 2;

	UPROPERTY(EditAnywhere, Category = "Special Room Settings", meta = (ClampMin = 1, EditCondition = bIsSpecialRoom))
	int32 Width = 2;

	UPROPERTY(EditAnywhere, Category = "Special Room Settings", meta = (ClampMin = 1, EditCondition = bIsSpecialRoom))
	int32 Length = 2;

	
	
protected:

	EConnection CalcRoomConnectionFromLocation(const FVector& Location) const;

	FORCEINLINE uint16 CalcIndexFromLocation(const FVector& Location) const; 
	
	virtual void BeginPlay() override;
	
	
#ifdef WITH_EDITOR
	
	virtual void OnConstruction(const FTransform& Transform) override;

	EConnection CalcRoomConnection(USceneComponent* RoomConnectionMarker);
	
#endif


};
