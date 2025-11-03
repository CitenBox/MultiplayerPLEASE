// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineGameState.h"

#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

void AOnlineGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOnlineGameState, ChatMessages);
}

void AOnlineGameState::OnRep_ChatMessages()
{
	OnChatUpdated.Broadcast();
}

void AOnlineGameState::SendMessage(APlayerController* Player, const FString& Message)
{
	if(!Player) return;

	FChatMessagesArray CopyArray;
	CopyArray.Messages.SetNum(ChatMessages.Messages.Num());
	
	for(FChatMessage ChatMessage : ChatMessages.Messages)
	{
		CopyArray.Messages[ChatMessage.Index] = ChatMessage;
	}

	ChatMessages.Messages = CopyArray.Messages;
	
	FChatMessage NewChatMessage;
	NewChatMessage.Index = ChatMessages.Messages.Num();
	NewChatMessage.Timestamp = FDateTime::Now();
	NewChatMessage.SenderName = Player->PlayerState->GetPlayerName();
	NewChatMessage.MessageText = Message;
	
	ChatMessages.Messages.Add(NewChatMessage);
	ChatMessages.MarkItemDirty(ChatMessages.Messages.Last());
	OnRep_ChatMessages();
}