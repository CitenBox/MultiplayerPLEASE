// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerHUD.h"

#include "ChatUI.h"


void UMultiplayerHUD::NativePreConstruct()
{
	Super::NativePreConstruct();

	if(ChatUI)
	{
		ChatUI->SetOwningPlayer(GetOwningPlayer());
	}
}

void UMultiplayerHUD::CloseChatUI()
{
}

void UMultiplayerHUD::OpenChatUI()
{
}

void UMultiplayerHUD::OpenInventoryUI()
{
}

void UMultiplayerHUD::CloseInventoryUI()
{
}
