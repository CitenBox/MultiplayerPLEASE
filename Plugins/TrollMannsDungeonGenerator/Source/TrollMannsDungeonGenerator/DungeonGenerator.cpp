// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"
#include "DungeonRoom.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

ADungeonGenerator::ADungeonGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

}

void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();

	int32 SeedResult = Seed;
	RandomStream.Initialize(SeedResult);


	//DungeonLayout.SetNumZeroed(Width*Length*Height);
	DungeonRoomLayout.SetNumZeroed(Width*Length*Height);
	
	UE_LOG(LogTemp, Warning, TEXT("Generating..."));
	Generate();


}


//for each mesh asset that will be in the dungeon we will have a instanced mesh component and here we are
//instancing meshes from the instanced mesh component for each transform that is associated with a specific mesh. (shef_x_thaul)
void ADungeonGenerator::SpawnDungeon()
{
	//todo: make it so it gets populated not in runtime but instead in editor time since it will be more performant
	TMap<UStaticMesh*, TArray<FTransform>> ReorganizedRoomData;
	
	for(uint32 i = 0; i < Width*Length*Height; i++)
	{
		UDungeonRoomDataAsset* Room = DungeonRoomLayout[i];
		if(!Room) continue;

		
		
		for(const FRoomComponentData& RoomComponentData : Room->RoomMeshInfoArray)
		{
			ReorganizedRoomData.FindOrAdd(RoomComponentData.StaticMesh).Add(RoomComponentData.Transform + FTransform(Calc3D(i) * RoomSize));
		}
	}
	
	for(auto MeshTransforms : ReorganizedRoomData)
	{
		if(UInstancedStaticMeshComponent** InstancedMeshComponent = DungeonMeshMap.Find(MeshTransforms.Key))
		{
			if(!*InstancedMeshComponent) continue;
			UInstancedStaticMeshComponent* CurrentInstancedMesh = *InstancedMeshComponent;
                         				
			CurrentInstancedMesh->AddInstances(MeshTransforms.Value, false, false);
		}
	}
}

void ADungeonGenerator::LayoutWithPathFinding()
{
	for(uint32 i = 0; i < Width*Length*Height; i++)
	{
		FVector temp = Calc3D(i);
		uint32 x = temp.X;
		uint32 y = temp.Y;
		uint32 z = temp.Z;

		
		
	}
}


UDungeonRoomDataAsset* ADungeonGenerator::SelectRoomType(int32 DungeonIndex)
{
	if(!DungeonRoomsDataAsset) return nullptr;

	TArray<UDungeonRoomDataAsset*> Out;
	DungeonRoomsDataAsset->FindRoomsWithFlags(Out, DungeonLayout[DungeonIndex]);

	//@todo currently we just choose a random room with matching connections however we probably want more depth to the selection process.
	if(Out.Num() > 0)
	{
		return Out[RandomStream.RandRange(0, Out.Num() - 1)];
	}
	else
	{
		return nullptr;
	}
}

int32 ADungeonGenerator::Offset(const FVector& NewOffset)
{
	return (CoordPath.Top().X + NewOffset.X) * Width + (CoordPath.Top().Y + NewOffset.Y) + (CoordPath.Top().Z + NewOffset.Z) * Width * Length;
}

int32 ADungeonGenerator::OffsetFrom(const FVector& From, const FVector& Offset)
{
	return (From.X + Offset.X) * Width + (From.Y + Offset.Y) + (From.Z + Offset.Z) * Width * Length;
}

void ADungeonGenerator::Generate()
{
	FRotator CurrentDungeonRotation = GetActorRotation();
}


void ADungeonGenerator::LayoutMazeAlgorithm()
{
	uint32 VisitedCount = 0;
	const FVector LEFT = FVector(1, 0, 0);
	const FVector RIGHT = FVector(-1, 0, 0);
	const FVector FORWARD = FVector(0, 1, 0);
	const FVector BACKWARD = FVector(0, -1, 0);
	const FVector UP = FVector(0, 0, 1);
	const FVector DOWN = FVector(0, 0, -1);

	
	//inclusive min and max
	const int32 x = RandomStream.RandRange(0, Width - 1);
	const int32 z = RandomStream.RandRange(0, Height - 1);
	const int32 y = 0;

	CoordPath.Push(FVector(x, y, z));

	DungeonLayout[Offset(FVector())] |= (int32)EConnection::VISITED;

	++VisitedCount;

	while (VisitedCount < RoomAmount)
	{
		TArray<int32> Neighbours;

		if (CoordPath.Top().X < Width - 1 && !(DungeonLayout[Offset(LEFT)] & (int32)EConnection::VISITED))
		{
			Neighbours.Add(0);
		}

		if (CoordPath.Top().X > 0 && !(DungeonLayout[Offset(RIGHT)] & (int32)EConnection::VISITED))
		{
			Neighbours.Add(1);
		}

		if (CoordPath.Top().Y < Length - 1 && !(DungeonLayout[Offset(FORWARD)] & (int32)EConnection::VISITED))
		{
			Neighbours.Add(2);
		}

		if (CoordPath.Top().Y > 0 && !(DungeonLayout[Offset(BACKWARD)] & (int32)EConnection::VISITED))
		{
			Neighbours.Add(3);
		}

		if (CoordPath.Top().Z < Height - 1 && !(DungeonLayout[Offset(UP)] & (int32)EConnection::VISITED))
		{
			Neighbours.Add(4);
		}

		if (CoordPath.Top().Z > 0 && !(DungeonLayout[Offset(DOWN)] & (int32)EConnection::VISITED))
		{
			Neighbours.Add(5);
		}


		if (Neighbours.IsEmpty())
		{
			CoordPath.Pop();

			if (CoordPath.Num() == 0)
			{
				break;
			}

			continue;
		}
		else
		{
			switch (Neighbours[RandomStream.RandRange(0, Neighbours.Num() - 1)])
			{
			case 0:
				DungeonLayout[Offset(LEFT)] |= (int32)EConnection::VISITED | (int32)EConnection::RIGHT;
				DungeonLayout[Offset(FVector())] |= (int32)EConnection::LEFT;
				CoordPath.Push(CoordPath.Top() + LEFT);
				break;
			case 1:
				DungeonLayout[Offset(RIGHT)] |= (int32)EConnection::VISITED | (int32)EConnection::LEFT;
				DungeonLayout[Offset(FVector())] |= (int32)EConnection::RIGHT;
				CoordPath.Push(CoordPath.Top() + RIGHT);
				break;
			case 2:
				DungeonLayout[Offset(FORWARD)] |= (int32)EConnection::VISITED | (int32)EConnection::BACKWARD;
				DungeonLayout[Offset(FVector())] |= (int32)EConnection::FORWARD;
				CoordPath.Push(CoordPath.Top() + FORWARD);
				break;
			case 3:
				DungeonLayout[Offset(BACKWARD)] |= (int32)EConnection::VISITED | (int32)EConnection::FORWARD;
				DungeonLayout[Offset(FVector())] |= (int32)EConnection::BACKWARD;
				CoordPath.Push(CoordPath.Top() + BACKWARD);
				break;
			case 4:
				DungeonLayout[Offset(UP)] |= (int32)EConnection::VISITED | (int32)EConnection::DOWN;
				DungeonLayout[Offset(FVector())] |= (int32)EConnection::UP;
				CoordPath.Push(CoordPath.Top() + UP);
				break;
			case 5:
				DungeonLayout[Offset(DOWN)] |= (int32)EConnection::VISITED | (int32)EConnection::UP;
				DungeonLayout[Offset(FVector())] |= (int32)EConnection::DOWN;
				CoordPath.Push(CoordPath.Top() + DOWN);
				break;
			}

			++VisitedCount;
		}
	}
}

void ADungeonGenerator::SyncDataToDungeonGenerator()
{
	if(!IsValid(DungeonRoomsDataAsset) ) return;
	
	DungeonMeshMap.Empty();
	
	for(const TTuple<UDungeonRoomDataAsset*, TSubclassOf<ADungeonRoom>>& RoomTypePair : DungeonRoomsDataAsset->RoomTypes)
	{
		if(!IsValid(RoomTypePair.Key)) continue;
		
		for(const FRoomComponentData& RoomComponentData: RoomTypePair.Key->RoomMeshInfoArray)
		{
			
			UInstancedStaticMeshComponent* InstancedComponent = Cast<UInstancedStaticMeshComponent>(AddComponentByClass(UInstancedStaticMeshComponent::StaticClass(), false, FTransform(), false));
			DungeonMeshMap.Add(RoomComponentData.StaticMesh, InstancedComponent);
		}
	}
}

//Order is: Width, Length
FORCEINLINE FVector2D ADungeonGenerator::Calc2D(int32 i) const
{
	return FVector2D(i / Length, i % Length);
}

//Order is: Width, Length Height.
FORCEINLINE FVector ADungeonGenerator::Calc3D(int32 i) const
{
	return FVector(i % Length , (i / Length) % Width, i / (Width * Length));
}

uint16 ADungeonGenerator::CalcFrom3D(uint16 X, uint16 Y, uint16 Z) const
{
	return (X % Length) + (Y % Width) * Length + (Z % Height) * Width * Length;
}

