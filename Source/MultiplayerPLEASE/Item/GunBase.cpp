// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBase.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


bool FFiringData::NetSerialize(FArchive& Ar, UPackageMap* Pack, bool& bOutSuccess)
{
	return true;
}

UGunBase::UGunBase()
{
}

void UGunBase::Fire()
{
	
	TArray<FHitResult> Hits;
	FVector OutLocation = FVector();
	FRotator OutRotation = FRotator();

	if(OuterPlayerControllerRef)
	{
		OuterPlayerControllerRef->GetPlayerViewPoint(OutLocation, OutRotation);
	}
	
	
	const FVector Start = OutLocation + AimOffset;
	const FVector ForwardVec = OutRotation.Vector();
	const FVector End = Start + (ForwardVec * BulletRange);

	FCollisionQueryParams CollisionQueryParams;
	
	if(OuterCharacterRef)
	{
		CollisionQueryParams.AddIgnoredActor(OuterCharacterRef);
		CollisionQueryParams.AddIgnoredComponent(OuterCharacterRef->GetMesh());
	}
	
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.f, 0, 1);

	GetWorld()->LineTraceMultiByChannel
	(
		Hits,
		Start,
		End,
		ECollisionChannel::ECC_Visibility,
		CollisionQueryParams
	);
	
	//UDamageType* DamageType = NewObject<UDamageType>(GunDamageTypeClass);
	
	
	for(FHitResult& E : Hits)
	{
		if(E.GetActor())
		{
			E.GetActor()->TakeDamage(Damage, FDamageEvent(), OuterPlayerControllerRef, OuterCharacterRef);
		}
	}

	if(FireEffectMuzzle)
	{
		
	}
}

void UGunBase::Action1(bool bActivate)
{
	if(bActivate)
	{
		GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &UGunBase::Fire, FireRate, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
	}
}

void UGunBase::Action2(bool bActivate)
{
	Super::Action2(bActivate);

	OuterCharacterRef->bUseControllerRotationYaw = !OuterCharacterRef->bUseControllerRotationYaw;
}

void UGunBase::DestroyItem()
{
	//GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
	Super::DestroyItem();
}

UGunBase::~UGunBase()
{
	UGunBase::DestroyItem();
}
