// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonRoomBuilder.h"


// Sets default values
ADungeonRoomBuilder::ADungeonRoomBuilder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
}

// Called when the game starts or when spawned
void ADungeonRoomBuilder::BeginPlay()
{
	
}

void ADungeonRoomBuilder::SyncDataToDungeonGenerator()
{
/*	UE_LOG(LogTemp, Warning, TEXT("SyncDataToDungeonGenerator called"));
	TArray<UObject*> DungeonRoomBuilders;
	GetObjectsOfClass(StaticClass(), DungeonRoomBuilders, true);

	for(UObject* obj : DungeonRoomBuilders)
	{
		if(ADungeonRoomBuilder* RoomBuilder =  Cast<ThisClass>(obj))
		{
			if(RoomBuilder)
			{
				RoomBuilder->OnSyncDataToDungeon();
			}
		}
	}*/
}

/*void ADungeonRoomBuilder::OnSyncDataToDungeon()
{
	if(IsValid(RoomData))
	{
		RoomData->RoomMeshInfoArray.Empty();
	}
	TArray<UActorComponent*> Components;
	GetComponents(Components, true);
	for(UActorComponent* Child : Components)
	{
		if(!IsValid(Child))
		{
			continue;
		}
 		
		if(Child->GetClass()->IsChildOf(UStaticMeshComponent::StaticClass()))
		{
			if(UStaticMeshComponent* Temp = Cast<UStaticMeshComponent>(Child))
			{
				FRoomComponentMeshInfo MeshInfo = FRoomComponentMeshInfo();
				if(IsValid(RoomData))
				{
					MeshInfo.Mesh = Temp->GetStaticMesh();
					MeshInfo.Transform = Temp->GetRelativeTransform();
					RoomData->RoomMeshInfoArray.Add(MeshInfo);
				}

			}
		}
	}
}*/
#ifdef WITH_EDITOR
void ADungeonRoomBuilder::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}


void ADungeonRoomBuilder::OnConstruction(const FTransform& Transform)
{
	UWorld* World = GetWorld();

	const float HalfRoomSize = RoomSize / 2;

	FVector Center = FVector(Transform.GetLocation() + FVector(0,0, HalfRoomSize));
	FVector Extent = FVector(HalfRoomSize, HalfRoomSize, HalfRoomSize);

	FlushPersistentDebugLines(World);
	DrawDebugBox(World, Center, Extent, FColor(255, 0, 0, 1), true);

	//OnSyncDataToDungeon();
}
#endif



