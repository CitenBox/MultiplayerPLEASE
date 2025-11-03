// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MultiplayerPLEASE/EGTypes.h"

#include "OnlineDatingFPAnimInstance.generated.h"

class AOnlineDatingSimCharacter;

UCLASS(Transient, Blueprintable, BlueprintType)
class MULTIPLAYERPLEASE_API UOnlineDatingFPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, meta=(ALlowPrivateAccess))
	AOnlineDatingSimCharacter* CharacterRef;

public:

	UOnlineDatingFPAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeInitializeAnimation() override;
	
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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetCharacterVelocity();
};
