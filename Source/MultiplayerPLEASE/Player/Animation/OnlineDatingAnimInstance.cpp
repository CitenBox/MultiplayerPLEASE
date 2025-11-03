// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineDatingAnimInstance.h"

#include "MultiplayerPLEASE/Player/OnlineDatingSimCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UOnlineDatingAnimInstance::UOnlineDatingAnimInstance()
{
	
}

void UOnlineDatingAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(CharacterMovementComponent)
	{
		Velocity = CharacterMovementComponent->Velocity;
		GroundSpeed = FVector(Velocity.X,Velocity.Y,0.f).Length();

		bShouldMove = !CharacterMovementComponent->GetCurrentAcceleration().IsZero() && GroundSpeed > 3.f;

		bIsFalling = CharacterMovementComponent->IsFalling();

		bIsCrouching = CharacterMovementComponent->IsCrouching();
	}
}

void UOnlineDatingAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	CharacterRef = Cast<AOnlineDatingSimCharacter>(GetOwningActor());
	
	if(CharacterRef)
	{
		CharacterRef->OnItemEquipped.AddUObject(this, &UOnlineDatingAnimInstance::OnItemEquipped);
		CharacterRef->OnItemUnEquipped.AddUObject(this, &UOnlineDatingAnimInstance::OnItemUnEquipped);

		CharacterMovementComponent = CharacterRef->CharacterMovementComponent;
	}
}





void UOnlineDatingAnimInstance::OnItemEquipped(EItemType ItemType)
{
	EquippedItemType = ItemType;
}

void UOnlineDatingAnimInstance::OnItemUnEquipped(EItemType ItemType)
{
	EquippedItemType = EItemType::None;
}

FVector UOnlineDatingAnimInstance::GetCharacterVelocity()
{
	return Velocity;
}
