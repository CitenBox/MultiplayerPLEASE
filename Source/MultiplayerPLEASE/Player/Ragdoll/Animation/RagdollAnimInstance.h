// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RagdollAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPLEASE_API URagdollAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	URagdollAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(BlueprintReadWrite)
	USkeletalMeshComponent* OtherCharacterMesh = nullptr;
};
