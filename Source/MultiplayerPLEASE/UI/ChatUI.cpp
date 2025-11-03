// Fill out your copyright notice in the Description page of Project Settings.

#include "ChatUI.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerPLEASE/Chat/OnlineGameState.h"

void UChatUI::NativeConstruct()
{
	Super::NativeConstruct();

	if(GetWorld())
	{
		OnlineGameState = GetWorld()->GetGameState<AOnlineGameState>();
		if(OnlineGameState)
		{
			OnlineGameState->OnChatUpdated.AddUniqueDynamic(this, &UChatUI::OnChatUpdated);
		}
	}
	
	if(ChatInputTextBox)
	{
		ChatInputTextBox->OnTextCommitted.AddUniqueDynamic(this, &UChatUI::OnTextCommitted);
	}

	if(ChatScrollBox)
	{
		ChatScrollBox->OnUserScrolled.AddUniqueDynamic(this, &UChatUI::OnUserScrolled);
	}
}

void UChatUI::StartInactivity()
{
	if(!GetWorld())return;
	GetWorld()->GetTimerManager().ClearTimer(InactivityTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(InactivityTimerHandle,[this]
	{
		if(OnAnimateClosingChat.IsBound())
		{
			OnAnimateClosingChat.Broadcast();
		}
	},
	InactivityTime,
	false,
	-1
	);
}

void UChatUI::OnAnimatedChatClosingComplete()
{
	if(OnFocusLost.IsBound())
	{
		OnFocusLost.Broadcast();
	}
}

void UChatUI::FormatMessageData(FString& OutMessage, const FString& PlayerName, const FString& Message)
{
	OutMessage.Append(PlayerName).Append(TEXT(": ")).Append(Message).Append(TEXT("\n\n"));
}

void UChatUI::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if(!ChatInputTextBox)return;
	
	if(CommitMethod == ETextCommit::OnEnter)
	{
		if(OnMessageSend.IsBound())
		{
			OnMessageSend.Broadcast(Text.ToString());
		}
		
		ChatInputTextBox->SetText(FText());
		if(OnFocusLost.IsBound())
		{
			OnFocusLost.Broadcast();
		}

		StartInactivity();
	}
}

void UChatUI::OnChatUpdated()
{
	if(OnlineGameState)
	{
		const FChatMessagesArray& MessagesArrayRef = OnlineGameState->GetChatMessagesArray();

		FString NewChat;
		TArray<FString> OrderedMessages;
		OrderedMessages.SetNumZeroed(MessagesArrayRef.Messages.Num());
		for(FChatMessage Message : MessagesArrayRef.Messages)
		{
			if(!OrderedMessages.IsValidIndex(Message.Index))
			{
				continue;
			}
			
			FormatMessageData(OrderedMessages[Message.Index], Message.SenderName, Message.MessageText); 
		}

		for(const FString& Message : OrderedMessages)
		{
			NewChat.Append(Message);
		}

		ChatTextBox->SetText(FText::FromString(NewChat));
		if(ChatScrollBox)
		{
			ChatScrollBox->ScrollToEnd();
		}
		SetVisibility(ESlateVisibility::Visible);
		
		StartInactivity();
	}
}

void UChatUI::OnUserScrolled(float CurrentOffset)
{
	StartInactivity();
}

void UChatUI::OpenChat()
{
	SetVisibility(ESlateVisibility::Visible);
	
	if(ChatTextBox)
	{
		ChatInputTextBox->SetKeyboardFocus();
	}

	if(!GetWorld())return;
	GetWorld()->GetTimerManager().ClearTimer(InactivityTimerHandle);
}

void UChatUI::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);

	if(OnFocusLost.IsBound())
	{
		OnFocusLost.Broadcast();
	}
	
	StartInactivity();
}
