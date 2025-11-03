// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Ragdoll.generated.h"

UCLASS()
class MULTIPLAYERPLEASE_API ARagdoll : public APawn
{
	GENERATED_BODY()

	friend class URagdollAnimInstance;


	
	
public:	
	// Sets default values for this actor's properties
	ARagdoll();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing=OnRep_bReplicateRagdoll)
	bool bReplicateRagdoll = false;
	UFUNCTION()
	void OnRep_bReplicateRagdoll();
	
	void StartRagdoll(ACharacter* NewCharacterRef);

	UFUNCTION()
	void DeleteRagdoll();

	UPROPERTY(EditAnywhere, Replicated, Category="Ragdoll")
	USkeletalMeshComponent* RagdollMeshComponent = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	ACharacter* CharacterRef = nullptr;

public:

};
