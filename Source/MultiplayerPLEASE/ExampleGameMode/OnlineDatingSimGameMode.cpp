// Copyright Epic Games, Inc. All Rights Reserved.

#include "OnlineDatingSimGameMode.h"

#include "GeometryTypes.h"
#include "OnlineSpawnPoint.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"


void AOnlineDatingSimGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AOnlineDatingSimGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}

void AOnlineDatingSimGameMode::RestartPlayerCustom(AController* NewPlayer)
{
	RestartPlayer(NewPlayer);
 //temporary
	/*
	if(GetWorld() == nullptr)
	{
		return;
	}

	if(!IsValid(NewPlayer))
	{
		return;
	}
	
	AActor* SpawnLocationActor;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), SpawnPoints);
	const int32 SpawnPointIndex = FMath::RandRange(0, SpawnPoints.Num());
	if(SpawnPoints.IsValidIndex(SpawnPointIndex) && IsValid(SpawnPoints[SpawnPointIndex]))
	{
		SpawnLocationActor = SpawnPoints[SpawnPointIndex];
	}
	else
	{
		SpawnLocationActor = ChoosePlayerStart(NewPlayer);
	}
	RestartPlayerAtPlayerStart(NewPlayer, SpawnLocationActor);
	*/
}

void AOnlineDatingSimGameMode::OnPlayerDeath(AController* Controller)
{
	FTimerHandle RespawnTimerHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &AOnlineDatingSimGameMode::RestartPlayerCustom, Controller);

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, TimerDel, RespawnTime, false);
	RespawnTimersMap.Add(Controller, RespawnTimerHandle);
}

void AOnlineDatingSimGameMode::OnPlayerDeath(UObjectBase* InUserObject, TDelegate<void()> InDelegate)
{
	FTimerHandle RespawnTimerHandle;
 	FTimerDelegate TimerDel;
 	TimerDel.BindLambda([&InDelegate]()
 	{
 		InDelegate.ExecuteIfBound();
 	});
 	GetWorldTimerManager().SetTimer(RespawnTimerHandle, TimerDel, RespawnTime, false);
}
