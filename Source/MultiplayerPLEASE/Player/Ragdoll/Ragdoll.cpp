// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.


#include "Ragdoll.h"

#include "MultiplayerPLEASE/Player/Ragdoll/Animation/RagdollAnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// Sets default values
ARagdoll::ARagdoll()
{
	bReplicates = true;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	RagdollMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RagdollMeshComponent"));

	
	//RagdollMeshComponent->bReplicatePhysicsToAutonomousProxy = true;
}

void ARagdoll::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	
	Params.bIsPushBased = true;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	Params.Condition = COND_None;
	DOREPLIFETIME_WITH_PARAMS_FAST(ARagdoll, bReplicateRagdoll, Params);
}

void ARagdoll::OnRep_bReplicateRagdoll()
{

	if(RagdollMeshComponent)
	{
		RagdollMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		
		
		RagdollMeshComponent->BodyInstance.SetInstanceSimulatePhysics(true, true, false);

		RagdollMeshComponent->SetPhysicsBlendWeight(1.f);
	}
}

void ARagdoll::StartRagdoll(ACharacter* NewCharacterRef)
{
	CharacterRef = NewCharacterRef;
	
	if(HasAuthority())
	{
		bReplicateRagdoll = true;
		MARK_PROPERTY_DIRTY_FROM_NAME(ARagdoll, bReplicateRagdoll, this);
		
		OnRep_bReplicateRagdoll();
	}
}

void ARagdoll::DeleteRagdoll()
{
	
}

// Called when the game starts or when spawned
void ARagdoll::BeginPlay()
{
	Super::BeginPlay();
	
}


