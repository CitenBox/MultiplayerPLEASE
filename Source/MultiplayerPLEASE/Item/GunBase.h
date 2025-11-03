// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "MultiplayerPLEASE/EGTypes.h"

#include "GunBase.generated.h"

struct FTimerHandle;

USTRUCT()
struct FFiringData
{
	GENERATED_BODY()

	uint8 FiredAmount;

	bool NetSerialize(FArchive& Ar, UPackageMap* Pack, bool& bOutSuccess);
};

template<>
struct TStructOpsTypeTraits<FFiringData> : public TStructOpsTypeTraitsBase2<FFiringData>
{
	enum {  WithNetSerializer = true, };
};

UCLASS()
class MULTIPLAYERPLEASE_API UGunBase : public UItemBase
{
	GENERATED_BODY()


	UGunBase();
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Settings")
	UNiagaraSystem* FireEffectMuzzle;

	FTimerHandle ShootingTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Gun Settings")
	float FireRate = 0.05;

	//not in meters
	UPROPERTY(EditAnywhere, Category = "Gun Settings")
	float BulletRange = 1000;

	UPROPERTY(EditAnywhere, Category = "Gun Settings")
	TSubclassOf<UDamageType> GunDamageTypeClass;

	UPROPERTY(EditAnywhere, Category = "Gun Settings")
	float Damage = 1.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Settings", meta=(AllowPrivateAccess))
	FVector AimOffset = FVector(0.f,0.f,0.f);
	
	
	UPROPERTY()
	bool bIsFiring = false;

	UFUNCTION()
	void Fire();

	//saves casting every time fire is called for now...

public:
	
	virtual void Action1(bool bActivate) override;

	virtual void Action2(bool bActivate) override;

	virtual void DestroyItem() override;

	virtual ~UGunBase() override;
	
};
