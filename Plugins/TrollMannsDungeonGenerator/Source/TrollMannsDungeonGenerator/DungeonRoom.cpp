// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonRoom.h"


// Sets default values
ADungeonRoom::ADungeonRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	
}


void ADungeonRoom::SyncData()
{
	if(!IsValid(RoomDataAsset)) return;

	RoomDataAsset->ConnectionFlagsMap.Empty();
	RoomDataAsset->ConnectionFlags = 0;

	RoomDataAsset->bIsSpecialRoom = bIsSpecialRoom;
	RoomDataAsset->Length = Length;
	RoomDataAsset->Width = Width;
	RoomDataAsset->Height = Height;

	RoomDataAsset->RoomMeshInfoArray.Empty();
	RoomDataAsset->ConnectionFlags = 0;
	RoomDataAsset->ConnectionFlagsMap.Empty();
	
	for(UActorComponent* Component : GetComponents())
	{
		if(Component->ComponentHasTag(RoomEntranceTag))
		{
			if(USceneComponent* SceneComponent = Cast<USceneComponent>(Component))
			{
				FVector ComponentRelativeLocation = SceneComponent->GetRelativeLocation();
				if(RoomDataAsset->bIsSpecialRoom)
				{
					FVector SpecialRoomIndex = ComponentRelativeLocation / RoomDataAsset->RoomSize;
					RoomDataAsset->ConnectionFlagsMap.FindOrAdd(CalcIndexFromLocation(ComponentRelativeLocation)) |= static_cast<int32>(CalcRoomConnectionFromLocation(ComponentRelativeLocation));
				}
				else
				{
					RoomDataAsset->ConnectionFlags |= static_cast<int32>(CalcRoomConnectionFromLocation(SceneComponent->GetRelativeTransform().GetLocation()));
				}
			}
		}
		else
		{
			if(UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Component))
			{
				FRoomComponentData RoomComponentData;
				RoomComponentData.Transform = StaticMeshComponent->GetComponentTransform();
				RoomComponentData.StaticMesh = StaticMeshComponent->GetStaticMesh();
            			
				RoomDataAsset->RoomMeshInfoArray.Add(RoomComponentData);
			}
		}
	}
}

EConnection ADungeonRoom::CalcRoomConnectionFromLocation(const FVector&  Location) const
{
	if(!RoomDataAsset) return EConnection::NONE;
	
	FVector LocationCopy = Location;
	if(RoomDataAsset->bIsSpecialRoom)
	{
		//LocationCopy = FMath::Modulo(Location, FVector(RoomDataAsset->RoomSize));
	}
	
	if(LocationCopy.X > RoomDataAsset->RoomSize - 100)
		return EConnection::FORWARD;

	if(LocationCopy.X < 100)
		return EConnection::BACKWARD;
	
	if(LocationCopy.Y > RoomDataAsset->RoomSize - 100)
		return EConnection::RIGHT;
	
	if(LocationCopy.Y < 100)
		return EConnection::LEFT;

	if(LocationCopy.Z > RoomDataAsset->RoomSize - 100)
		return EConnection::UP;

	if(LocationCopy.Z < 100)
		return EConnection::DOWN;

	return EConnection::NONE;
}

uint16 ADungeonRoom::CalcIndexFromLocation(const FVector& Location) const
{
	if(!RoomDataAsset) return 0;

	return FMath::Clamp(((int32)Location.X + 100) / RoomDataAsset->RoomSize, 0, RoomDataAsset->Length) +
			FMath::Clamp(((int32)Location.Y + 100) / RoomDataAsset->RoomSize * RoomDataAsset->Length, 0, RoomDataAsset->Width) +
			FMath::Clamp(((int32)Location.Z + 100) / RoomDataAsset->RoomSize * RoomDataAsset->Width * RoomDataAsset->Length, 0, RoomDataAsset->Height);
}

// Called when the game starts or when spawned
void ADungeonRoom::BeginPlay()
{
	
}



#ifdef WITH_EDITOR
void ADungeonRoom::OnConstruction(const FTransform& Transform)
{
	
	SyncData();
	UWorld* World = GetWorld();
	
	const double HalfRoomSize = RoomDataAsset->RoomSize / 2;
	const int32 RoomSize = RoomDataAsset->RoomSize;

	FVector Center = bIsSpecialRoom
	?
		FVector(Transform.GetLocation() + FVector(HalfRoomSize * Length,HalfRoomSize * Width, HalfRoomSize * Height))
	:
		FVector(Transform.GetLocation() + FVector(HalfRoomSize, HalfRoomSize, HalfRoomSize))
	;
	
	FVector Extent = Center;

	for(UActorComponent* RoomComponent : GetComponents())
	{
		if(!IsValid(RoomComponent)) continue;
		if(!RoomComponent->ComponentHasTag(RoomEntranceTag)) continue;
		
		if(USceneComponent* RoomEntranceComponent = Cast<USceneComponent>(RoomComponent))
		{
			// @todo: cant update location through code without it breaking where the component is visually
			//FVector FinalLocation = ClampVector(RoomEntranceComponent->GetRelativeLocation(), FVector(0), FVector(RoomSize));
			//FinalLocation = FinalLocation.GridSnap(HalfRoomSize);
			//RoomEntranceComponent->SetRelativeLocation(FinalLocation);
		}
	}

	FlushPersistentDebugLines(World);
	DrawDebugBox(World, Center, Extent, FColor(255, 0, 0, 1), true);
	
}

//@Todo: yet to be implemented
EConnection ADungeonRoom::CalcRoomConnection(USceneComponent* RoomConnectionMarker)
{
	return EConnection::NONE;
}
#endif


