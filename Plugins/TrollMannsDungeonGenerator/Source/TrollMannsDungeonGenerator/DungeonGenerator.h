// Fill out your copyright notice in the Description page of Project Settings.

/*
						  ~7:                                ^7~
						:7~                                    ^7~
					   ^?.                                       ~7^
					  ^?                                           !!
					  ?.                                            !!
					 ~!                                              7~
					.?                                                ?:
					~~               .~                 .::.          .J
					?:               .7             .!YB&&&#P?:        ^!
				   .?                :!          .!P&@@@@@@@@@@5^^~~~~~~5^
				   ^!                ..    :^~75B&@@#PYJJ?7?J5B@@@&#BGPYY5
				   ~^^J5PPPY!:...          5@@@@#P?^^!~!^^^.^!^!B^:.     7^
				   !?@@@@@@@@@&#&J^77!^~~7JB&?!^   ~!.7?G5!! .?^!~       .J
				.^~JPBY?JJ?77YPGY. ?P!^^^~7JP~     ?^~77PY~!^~!^ ?        ~7
				P@@@~  !^:!!!!^^~!.?         ?      ~~^^^:^^^^:  ?^        7^
				?@B5! .? J!#&?J^~!7^         ~!      ..:::^^^^~^^^         .?
				 G^ 7  !~7~~^::^.!!           ~~^^^^^^^:::...               7~
				 7? ?  :!~~!!7?777                                           J.
				  ~!?^^^^^^^:.  ?.                                           ~!
				   !^          .?                   :7.                      .7
				   7.          ^!           .^~~:    :7:                      ?
				   ?           .?..       ^G&@@@&:     !!                     ?.
				   7            ~Y?P~     ^B&&#G7       ^7:                  :G5:
				   ?            ^?7J7       ..           .7^                 .PG!
				   7:           !:.!.                      !~                ~G#5:
				   JBJ^         ?.                          ^7.              :Y&P7
				  .JBB!        :7             ....           :?:             .Y#GY.
				  :?5#Y.       !^    :^^^^^^~^^^^^~~^.        .7^            .5##7.
				   7BB&Y.      ?.  ~!^...:::::::::::.           7^           7G&PJ^
				   :JPBB7.     ?. .. ^~~J^::57..~7.^?~:.         7^          ~GGY7.
					.JGBP!     ?   ^BG. ?:  !!  :P?J&5##5!        7~        ~5#GY.
					 ~BGB?    .7   5@@BP#BPG&&B#&@@@@@@@@@B~       ?.      :JBBJ~
					  !B&B!.  .7   .#@@@@@@@@@@@@@@@@@@@@@@@5^     :7      ?BBP?~^^^:.
					.:^5#&P!  .7    7@@@@@@@@@@@@@@@@@@@@@@@@@!     7^    ~Y#GJ~. .::^~~^:..
				.^~~~^.7PG#P~ .7     G@@@@@@@@@@@@@@@@@@@@@@@@Y      ?    !GB57         .:^~~~^:.
			.:~~~:.    .755BP^ ?     ~@@@@@@@@@@@@@@@@@@@@@@@&^      !^  ^J#BP:              .:^^~~~
	   .:^~~^:.          :?YGG.~!     J@@@@@@@@@@@@@@@@@@@@@@Y       ^^ :?BG5?                     .
  .^~~~^:.               .?GB&Y.!^    .B@@@@@@@@@@@@@@@@@#&@B           7G&57~
~~^:.                      7G#@P7^     :B@@#P@@@@@@@@@@@B~~B^          :Y#PY:
							^JPBB7      :B@#7#@@@@@@&&PP!?!!           !P#5:
							 :5P##!      .?#Y55Y?5!J~!!!?7:  ^5~     .J#G!.
							  ^YPBG?  .?~   ^~!!!!~~7^^.   .?#P7     ~P#?.
								~Y#B7:.5BG?^.          :~7JPBGP.    7##5?.
								 ~P##Y::J#PBBPYJYJYJYY5GB5P?7.    :J#&P~.
								  ^JB#5^:!^?5J?J5GJY?7?!!:      :JY#GJ7
								   :JG&GJ::.    ....           :5BBP7.
									 !55BGPY~..               7#5YJ^
*/


#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "TrollMannsDungeonTypes.h"
#include "DungeonGenerator.generated.h"

class UDungeonRoomDataAsset;
class UDungeonRoomsDataAsset;
class UDungeonRoomData;
class USpecialDungeonRoomData;
class UInstancedStaticMeshComponent;
class UMPGameInstance;
class ADungeonGenerator;
class ASpecialDungeonRoom;
class ADungeonRoom;
class ASpecialDungeonRoom;

UCLASS()
class TROLLMANNSDUNGEONGENERATOR_API ADungeonGenerator : public AActor
{
	GENERATED_BODY()

public:

	ADungeonGenerator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonGenerator")
		int32 Seed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonGenerator")
	bool bLocalSeed = false;
	
	UPROPERTY(BlueprintReadOnly)
	FRandomStream RandomStream;

	UPROPERTY(EditAnywhere, Category = "DungeonGenerator", meta = (ClampMin = 1))
	uint32 Height = 20;

	UPROPERTY(EditAnywhere, Category = "DungeonGenerator", meta = (ClampMin = 1))
	uint32 Width = 20;

	UPROPERTY(EditAnywhere, Category = "DungeonGenerator", meta = (ClampMin = 1))
	uint32 Length = 20;

	UPROPERTY(EditAnywhere, Category = "DungeonGenerator", meta = (ClampMin = 1))
	uint32 RoomAmount = Height * Width * Length;

	UPROPERTY(EditAnywhere, Category = "DungeonGenerator", meta = (ClampMin = 1))
	uint32 SpecialRoomAmount = 30;

	UPROPERTY(EditAnywhere, Category = "DungeonGenerator", meta = (ClampMin = 1))
	int32 RoomSize = 1000;

	UPROPERTY()
	TArray<int32> DungeonLayout;

	UPROPERTY()
	TArray<UDungeonRoomDataAsset*> DungeonRoomLayout;
	
	UPROPERTY()
	TMap<int32, AActor*> RoomRefs;

	UPROPERTY()
	TArray<FVector> CoordPath;

	UFUNCTION(CallInEditor, Category = "DungeonGenerator")
	void SyncDataToDungeonGenerator();

	UPROPERTY(EditAnywhere, Category = "DungeonGenerator")
	UDungeonRoomsDataAsset* DungeonRoomsDataAsset = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	TMap<UStaticMesh*, UInstancedStaticMeshComponent*> DungeonMeshMap;

	FORCEINLINE FVector2D Calc2D(int32 i) const;

	FORCEINLINE FVector Calc3D(int32 i) const;

	FORCEINLINE uint16 CalcFrom3D(uint16 X, uint16 Y, uint16 Z) const;

	FORCEINLINE virtual int32 OffsetFrom(const FVector& From, const FVector& Offset);
protected:

	virtual void BeginPlay() override;

	virtual void SpawnDungeon();

	virtual void LayoutWithPathFinding();

	FORCEINLINE virtual UDungeonRoomDataAsset* SelectRoomType(int32 DungeonIndex);

	FORCEINLINE virtual int32 Offset(const FVector& NewOffset);

	virtual void Generate();

	virtual void LayoutMazeAlgorithm();


};
