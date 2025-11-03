// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MultiplayerPLEASE/Interface/Interactable.h"
#include "MultiplayerPLEASE/EGTypes.h"

#include "BasePickup.generated.h"

class UItemInfoDataAsset;

UCLASS()
class MULTIPLAYERPLEASE_API ABasePickup : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePickup();
	
	//item id for retrieving iteminfo struct from data asset
	UPROPERTY(EditAnywhere, Category="ItemInfo")
    int32 ItemIDForSyncing;
	
	UPROPERTY(EditAnywhere, Category = "DataAsset")
	UItemInfoDataAsset* ItemInfoDataAsset;

	UPROPERTY(EditAnywhere, Category = "ItemInfo")
	FEGItemInfo ItemInfo;
    
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PickupMesh;
protected:
	
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*
	* supposed to work but unreal being unreal just causes issues. this was supposed to allow you to edit a pickup's
	* item id and it will automatically fill in the rest of the fields from the data asset but the project wont package with this
	* even with the preprocessor directive, which is exactly how you are supposed to use PostEditChangeProperty callbacks...
	*
	*
#ifdef WITH_EDITOR
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
	*/

public:	
	
virtual void Interact(UObject* Caller) override;


};
