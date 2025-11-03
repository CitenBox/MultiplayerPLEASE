// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataAsset.h"
#include "TrollMannsDungeonGenerator/TrollMannsDungeonTypes.h"
#include "DungeonRoomBuilder.generated.h"



UCLASS()
class TROLLMANNSDUNGEONGENERATORTOOLS_API ADungeonRoomBuilder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonRoomBuilder();

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1))
	int32 RoomSize = 1000;

	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = EConnection))
	int32 ConnectionFlags = (int32)EConnection::NONE;
		

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	
	//virtual void OnSyncDataToDungeon();

	UFUNCTION(CallInEditor, Category="Utility")
	static void SyncDataToDungeonGenerator();

	
#ifdef WITH_EDITOR
	
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
#endif
};


