// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/NamedSlot.h"

#include "MultiplayerHUD.generated.h"

class UInventoryUI;
class UHealthUI;
class UNamedSlot;
class UChatUI;

UCLASS()
class MULTIPLAYERPLEASE_API UMultiplayerHUD : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativePreConstruct() override;
	
	public:


	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UInventoryUI* InventoryUI;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UInventoryUI* HotbarUI;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UHealthUI* HealthUI;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UNamedSlot* TradeNotificationSlot;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UChatUI* ChatUI;

	void CloseChatUI();
	void OpenChatUI();

	void OpenInventoryUI();
	void CloseInventoryUI();
};
