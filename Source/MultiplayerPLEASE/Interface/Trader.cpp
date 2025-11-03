// Fill out your copyright notice in the Description page of Project Settings.


#include "Trader.h"

class UTradeHandler;

UTradeHandler* ITrader::GetTradeHandler()
{
	return nullptr;
}

// Add default functionality here for any ITrader functions that are not pure virtual.
FString ITrader::GetTraderName()
{
	return FString();
}

int32 ITrader::GetTraderID()
{
	return int32();
}
