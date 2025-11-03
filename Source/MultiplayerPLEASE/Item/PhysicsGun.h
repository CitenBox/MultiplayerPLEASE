// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerPLEASE/Item/ItemBase.h"

#include "Tickable.h"

#include "PhysicsGun.generated.h"

class UPhysicsHandleComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class MULTIPLAYERPLEASE_API UPhysicsGun : public UItemBase,  public FTickableGameObject
{
	GENERATED_BODY()
public:

	virtual void DoneInitiliaze() override;

protected:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;

	//not in meters
	UPROPERTY(EditAnywhere, Category = "Physics Gun Settings")
	float GrabRange = 1000;

	UPROPERTY()
	bool bIsGrabbing = false;

	UPROPERTY(Replicated)
	float OriginalDistance;

	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandleComp = nullptr;
	
	virtual void Action1(bool bActivate) override;

	virtual void Action2(bool bActivate) override;
};
