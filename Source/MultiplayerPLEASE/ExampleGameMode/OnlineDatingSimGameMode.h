// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerPLEASE/Player/OnlineDatingSimCharacter.h"
#include "OnlineDatingSimGameMode.generated.h"

class AOnlineSpawnPoint;

UCLASS()
class AOnlineDatingSimGameMode : public AGameModeBase
{
	GENERATED_BODY()


protected:

public:

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void RestartPlayer(AController* NewPlayer) override;
	
	UPROPERTY()
	TMap<AController*, FTimerHandle> RespawnTimersMap;

	UPROPERTY(EditAnywhere)
	float RespawnTime = 5.f;

	UPROPERTY()
	TArray<AActor*> SpawnPoints;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> PawnClass = AOnlineDatingSimCharacter::StaticClass();

	virtual void RestartPlayerCustom(AController* NewPlayer);
	
	virtual void OnPlayerDeath(AController* Controller);
	virtual void OnPlayerDeath(UObjectBase* InUserObject, TDelegate<void()> InDelegate);
};



