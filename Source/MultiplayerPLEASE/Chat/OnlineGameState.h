// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ChatMessage.h"
#include "OnlineGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChatUpdated);

UCLASS()
class MULTIPLAYERPLEASE_API AOnlineGameState : public AGameStateBase
{
	GENERATED_BODY()

	
protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_ChatMessages)
	FChatMessagesArray ChatMessages;
	UFUNCTION()
	void OnRep_ChatMessages();
	
public:
	
	const FChatMessagesArray& GetChatMessagesArray() const { return ChatMessages; }

	FOnChatUpdated OnChatUpdated;

	UFUNCTION()
	void SendMessage(APlayerController* Player ,const FString& Message = TEXT(""));
};
