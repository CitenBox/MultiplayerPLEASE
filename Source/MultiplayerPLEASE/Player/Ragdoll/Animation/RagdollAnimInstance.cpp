// Fill out your copyright notice in the Description page of Project Settings.


#include "RagdollAnimInstance.h"

#include "GameFramework/Character.h"
//#include "MultiplayerPLEASE/Player/Ragdoll/Ragdoll.h"

URagdollAnimInstance::URagdollAnimInstance()
{
	//RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
}

void URagdollAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

/*	if(ARagdoll* Ragdoll = Cast<ARagdoll>(TryGetPawnOwner()))
	{
		if(Ragdoll->CharacterRef)
		{
			OtherCharacterMesh = Ragdoll->CharacterRef->GetMesh();
		}
	}*/
}
