// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MultiplayerPLEASE/EGTypes.h"

#include "OnlineDatingAnimInstance.generated.h"

class AOnlineDatingSimCharacter;
class UCharacterMovementComponent;

UCLASS(Transient, Blueprintable, BlueprintType)
class MULTIPLAYERPLEASE_API UOnlineDatingAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UPROPERTY()
	AOnlineDatingSimCharacter* CharacterRef;

public:

	UOnlineDatingAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeInitializeAnimation() override;

	UPROPERTY(BlueprintReadOnly, Category="Player")
	UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY(BlueprintReadOnly, Category="Player")
	bool bIsCrouching = false;
	
	UPROPERTY(BlueprintReadWrite)
	float Speed = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector Velocity;

	UPROPERTY(BlueprintReadWrite)
	float GroundSpeed = 0.f;

	UPROPERTY(BlueprintReadWrite)
	bool bShouldMove = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Settings")
	EItemType EquippedItemType = EItemType::None;


	UFUNCTION()
	void OnItemEquipped(EItemType ItemType);

	UFUNCTION()
	void OnItemUnEquipped(EItemType ItemType);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe))
	FVector GetCharacterVelocity();
};
