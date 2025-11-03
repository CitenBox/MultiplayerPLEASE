// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineDatingFPAnimInstance.h"

#include "MultiplayerPLEASE/Player/OnlineDatingSimCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UOnlineDatingFPAnimInstance::UOnlineDatingFPAnimInstance()
{
	
}

void UOnlineDatingFPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(CharacterRef)
	{
		GroundSpeed = CharacterRef->GetCharacterMovement()->Velocity.Length();

		bShouldMove = CharacterRef->GetVelocity().Length() > 0.f;

		bIsFalling = CharacterRef->GetCharacterMovement()->IsFalling();
	}
	
}

void UOnlineDatingFPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	CharacterRef = Cast<AOnlineDatingSimCharacter>(GetOwningActor());
	
	if(CharacterRef)
	{
		CharacterRef->OnItemEquipped.AddUObject(this, &UOnlineDatingFPAnimInstance::OnItemEquipped);
		CharacterRef->OnItemUnEquipped.AddUObject(this, &UOnlineDatingFPAnimInstance::OnItemUnEquipped);
	}
}

void UOnlineDatingFPAnimInstance::OnItemEquipped(EItemType ItemType)
{
	EquippedItemType = ItemType;
}

void UOnlineDatingFPAnimInstance::OnItemUnEquipped(EItemType ItemType)
{
	EquippedItemType = EItemType::None;
}

FVector UOnlineDatingFPAnimInstance::GetCharacterVelocity()
{
	if(CharacterRef)
	{
		return CharacterRef->GetVelocity();
	}

	return FVector();
}
