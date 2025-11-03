// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ChatMessage.generated.h"

USTRUCT()
struct MULTIPLAYERPLEASE_API FChatMessage : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	int16 Index = -1;

	UPROPERTY(VisibleAnywhere)
	FString SenderName;
	
	UPROPERTY(VisibleAnywhere)
	FString MessageText;

	UPROPERTY(NotReplicated)
	FDateTime Timestamp;

	FChatMessage() : SenderName(TEXT("Unknown")), MessageText(TEXT("")), Timestamp(FDateTime::Now()) {}
};

USTRUCT()
struct MULTIPLAYERPLEASE_API FChatMessagesArray : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FChatMessage> Messages;
	bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParms)
	{
		return FastArrayDeltaSerialize<FChatMessage>( Messages, DeltaParms, *this);
	}
};

template<>
struct TStructOpsTypeTraits<FChatMessagesArray> : TStructOpsTypeTraitsBase2<FChatMessagesArray>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};