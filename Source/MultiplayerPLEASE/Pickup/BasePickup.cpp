// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickup.h"
#include "MultiplayerPLEASE/Inventory/Inventory.h"
#include "MultiplayerPLEASE/Item/ItemInfoDataAsset.h"
#include "MultiplayerPLEASE/Interface/ItemTransfer.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABasePickup::ABasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(true);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetSimulatePhysics(true);
	PickupMesh->SetIsReplicated(true);
	PickupMesh->bReplicatePhysicsToAutonomousProxy = true;
}

// Called when the game starts or when spawned
void ABasePickup::BeginPlay()
{
	Super::BeginPlay();
}

void ABasePickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

/*
#ifdef WITH_EDITOR
void ABasePickup::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	if(ItemInfoDataAsset)
	{
		ItemInfo = ItemInfoDataAsset->GetItemInfo(ItemInfo.ID);
	}
}
#endif
*/

void ABasePickup::Interact(UObject* Caller)
{
	if (IItemTransfer* Player = Cast<IItemTransfer>(Caller))
	{
		Player->GiveItem(ItemInfo);
		
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);
		
		PickupMesh = nullptr;
		
		Destroy();
	}
}

