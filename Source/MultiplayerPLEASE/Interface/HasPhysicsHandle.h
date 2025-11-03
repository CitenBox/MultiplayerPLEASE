// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HasPhysicsHandle.generated.h"

class UPhysicsHandleComponent;
// This class does not need to be modified.
UINTERFACE()
class UHasPhysicsHandle : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
UPhysicsHandleComponent;
class MULTIPLAYERPLEASE_API IHasPhysicsHandle
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual UPhysicsHandleComponent* GetPhysicsHandleComponent() const = 0;
};
