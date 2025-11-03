// Fill out your copyright notice in the Description page of Project Settings.


#include "TradeMenu.h"

#include "MultiplayerPLEASE/Interface/Trader.h"


void UTradeMenu::UpdateTradeRequests(const TArray<UObject*>& TradeRequests)
{
	UpdateTradeRequestsBlueprint(TradeRequests);
}
