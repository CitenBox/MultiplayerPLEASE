// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TradeMenu.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRequestResponse, UObject*, Trader, bool, bAccepted);

UCLASS(Blueprintable)
class MULTIPLAYERPLEASE_API UTradeMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	
	FOnRequestResponse OnRequestResponse;
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTradeRequestsBlueprint(const TArray<UObject*>& TradeRequests);

	void UpdateTradeRequests(const TArray<UObject*>& TradeRequests);

	

};
