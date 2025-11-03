// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerPLEASE/Item/PhysicsGun.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"
//#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerPLEASE/Interface/HasPhysicsHandle.h"
#include "Net/UnrealNetwork.h"


void UPhysicsGun::DoneInitiliaze()
{
	Super::DoneInitiliaze();
	if(OuterCharacterRef)
	{
		PhysicsHandleComp = Cast<IHasPhysicsHandle>(OuterCharacterRef)->GetPhysicsHandleComponent();
	}
}

void UPhysicsGun::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;

	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, OriginalDistance, Params);
}

TStatId UPhysicsGun::GetStatId() const
{
	return TStatId();
}

void UPhysicsGun::Tick(float DeltaTime)
{
	if(bIsGrabbing && PhysicsHandleComp)
	{
		FVector OutLocation = FVector();
		FRotator OutRotation = FRotator();
        
		if(OuterPlayerControllerRef)
		{
			OuterPlayerControllerRef->GetPlayerViewPoint(OutLocation, OutRotation);
		}
		
		const FVector Start = OutLocation;
		const FVector ForwardVec = OutRotation.Vector();
		const FVector End = Start + (ForwardVec * OriginalDistance);
		
		PhysicsHandleComp->SetTargetLocationAndRotation(End, OutRotation);
	}
}

void UPhysicsGun::Action1(bool bActivate)
{
	Super::Action1(bActivate);

	if(!IsValid(PhysicsHandleComp) || !IsValid(OuterCharacterRef))
	{
		return;
	}

	if(bActivate && !bIsGrabbing)
	{
		TArray<FHitResult> Hits;
		FVector OutLocation = FVector();
		FRotator OutRotation = FRotator();
        
		if(OuterPlayerControllerRef)
		{
			OuterPlayerControllerRef->GetPlayerViewPoint(OutLocation, OutRotation);
		}
        	
        	
		const FVector Start = OutLocation;
		const FVector ForwardVec = OutRotation.Vector();
		const FVector End = Start + (ForwardVec * GrabRange);
        
        FCollisionQueryParams CollisionQueryParams;
        	

		CollisionQueryParams.AddIgnoredActor(OuterCharacterRef);
		CollisionQueryParams.AddIgnoredComponent(OuterCharacterRef->GetMesh());

        	
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.f, 0, 1);
        
		GetWorld()->LineTraceMultiByChannel
		(
		Hits,
		Start,
		End,
		ECC_Visibility,
		CollisionQueryParams
		);
        
        
		if(Hits.IsValidIndex(0))
		{
			auto HitComponent = Hits[0].GetComponent();
			if(IsValid(HitComponent))
			{
				PhysicsHandleComp->GrabComponentAtLocationWithRotation(HitComponent, FName("None"),HitComponent->GetComponentLocation(), FRotator());
				if(PhysicsHandleComp->GetGrabbedComponent())
				{
					OriginalDistance = FVector::Distance(HitComponent->GetComponentLocation(),  OuterCharacterRef->GetActorLocation());
					MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, OriginalDistance, this)
					bIsGrabbing = true;
				}
			}
        		
		}
	}
	else
	{
		PhysicsHandleComp->ReleaseComponent();
		bIsGrabbing = false;
	}
}

void UPhysicsGun::Action2(bool bActivate)
{
	Super::Action2(bActivate);
}
