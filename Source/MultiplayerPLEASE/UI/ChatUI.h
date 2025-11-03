// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "ChatUI.generated.h"

class UScrollBox;
class AOnlineDatingCharacterController;
class UTextBlock;
class UEditableText;
class AOnlineGameState;

DECLARE_MULTICAST_DELEGATE(FOnFocusLost);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMessageSend, const FString& Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimateClosingChat);

UCLASS(Blueprintable)
class MULTIPLAYERPLEASE_API UChatUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnChatUpdated();

	UFUNCTION()
	void OnUserScrolled(float CurrentOffset);
	virtual void NativeConstruct() override;

	
	void StartInactivity();

	//time until the chat closes after a certain amount of time of inactivity.
	UPROPERTY(EditAnywhere)
	float InactivityTime = 10.f;

	FTimerHandle InactivityTimerHandle;

	UFUNCTION(BlueprintCallable)
	void OnAnimatedChatClosingComplete();

public:

	UPROPERTY()
	AOnlineGameState* OnlineGameState;
	
	UFUNCTION(BlueprintCallable)
	void FormatMessageData(FString& OutMessage, const FString& PlayerName, const FString& Message);

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ChatInputTextBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ChatTextBox;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ChatScrollBox;

	FOnFocusLost OnFocusLost;
	
	FOnMessageSend OnMessageSend;
	
	UPROPERTY(BlueprintAssignable)
	FOnAnimateClosingChat OnAnimateClosingChat;

	void OpenChat();

	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
};
