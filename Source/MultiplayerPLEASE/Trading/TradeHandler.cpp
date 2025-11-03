// Fill out your copyright notice in the Description page of Project Settings.


#include "TradeHandler.h"

#include "MultiplayerPLEASE/Interface/Trader.h"
#include "MultiplayerPLEASE/Inventory/Inventory.h"
#include "Net/UnrealNetwork.h"

void UTradeHandler::BeginDestroy()
{
	
	Super::BeginDestroy();
}

void UTradeHandler::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, ThisTradeInventory, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, OtherTradeInventory, Params);

	Params.Condition = COND_OwnerOnly;
	Params.bIsPushBased = true;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, IncomingTradeRequests, Params);
}

void UTradeHandler::StartTrade()
{
	static bool DoOnce = true;
	if(OtherTradeHandler.Get() && DoOnce)
	{
		DoOnce = false;
		OtherTradeHandler.Get()->StartTrade();
	}
}

void UTradeHandler::BeginPlay()
{
	Super::BeginPlay();
	
	ThisTraderRef = GetOuter();
}

UTradeHandler::UTradeHandler()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTradeHandler::InitTradeHandler(UObject* ThisTrader, UObject* OtherTrader, UInventory* ThisInventory)
{
	ThisTraderRef = ThisTrader;
	OtherTraderRef = OtherTrader;
	ThisPlayerInventoryRef = ThisInventory;
}

void UTradeHandler::CancelTrade()
{
	static bool DoOnce = true;
	if(OtherTradeHandler.Get() && DoOnce)
	{
		DoOnce = false;
		OtherTradeHandler.Get()->CancelTrade();
	}

	DestroyComponent();
}

void UTradeHandler::OnRep_IncomingTradeRequests()
{
	OnUpdateIncomingTradeRequests.Broadcast(IncomingTradeRequests);
}

int32 UTradeHandler::FindIncomingTradeRequest(UObject* Trader)
{
	for(int32 i = 0; UObject* ExistingTrader : IncomingTradeRequests)
	{
		if(ExistingTrader == Trader)
		{
			return i;
		}
		
		i++;
	}

	return -1;
}

int32 UTradeHandler::FindOutGoingTradeRequest(UObject* Trader)
{
	for(int32 i = 0; UObject* ExistingTrader : IncomingTradeRequests)
	{
		if(ExistingTrader == Trader)
		{
			return i;
		}
		
		i++;
	}

	return -1;
}

void UTradeHandler::RespondToTrade(UObject* Trader, bool bAccepted)
{
	if(GetOwner())
	{
		if(!GetOwner()->HasLocalNetOwner())
		{
			return;
		}
	}
	
	if(bAccepted)
	{
		OnTradeResponseAcceptedRPCServer(Trader);
	}
	else
	{
		OnTradeResponseDeclinedRPCServer(Trader);
	}
}

void UTradeHandler::OnTradeResponseAcceptedRPCServer_Implementation(UObject* Trader)
{
	const int32 FoundIndex = FindIncomingTradeRequest(Trader);

	if(IncomingTradeRequests.IsValidIndex(FoundIndex) == false)
	{
		return;
	}
	ITrader* FoundTrader = Cast<ITrader>(IncomingTradeRequests[FoundIndex]);
	if(!FoundTrader)
	{
		return;
	}

	if(UTradeHandler* TradeHandler = FoundTrader->GetTradeHandler())
	{
		TradeHandler->OnTradeResponse(ThisTraderRef, true);
	}
}

void UTradeHandler::OnTradeResponseDeclinedRPCServer_Implementation(UObject* Trader)
{
	const int32 FoundIndex = FindIncomingTradeRequest(Trader);

	if(IncomingTradeRequests.IsValidIndex(FoundIndex) == false)
	{
		return;
	}
	ITrader* FoundTrader = Cast<ITrader>(IncomingTradeRequests[FoundIndex]);
	if(!FoundTrader)
	{
		return;
	}

	if(UTradeHandler* TradeHandler = FoundTrader->GetTradeHandler())
	{
		TradeHandler->OnTradeResponse(ThisTraderRef, false);
	}
}

void UTradeHandler::TradeRequest(UObject* Trader)
{
	const int32 FoundIndex = FindIncomingTradeRequest(Trader);

	if(IncomingTradeRequests.IsValidIndex(FoundIndex) == false)
	{
		return;
	}
	ITrader* FoundTrader = Cast<ITrader>(IncomingTradeRequests[FoundIndex]);
	if(!FoundTrader)
	{
		return;
	}

	if(UTradeHandler* TradeHandler = FoundTrader->GetTradeHandler())
	{
		TradeHandler->OnTradeRequest(ThisTraderRef);
	}
}

void UTradeHandler::OnTradeRequest(UObject* Caller)
{
	if(Caller == nullptr)
	{
		return;
	}
	
	IncomingTradeRequests.Add(Caller);
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, IncomingTradeRequests, this);
	OnRep_IncomingTradeRequests();
}

void UTradeHandler::OnTradeResponse(UObject* Caller, bool bAccepted)
{
	const int32 FoundIndex = FindOutGoingTradeRequest(Caller);
	if(OutGoingTradeRequests.IsValidIndex(FoundIndex))
	{
		OutGoingTradeRequests.RemoveAt(FoundIndex);
	}
	
	if(bAccepted)
	{
		StartTrade();
	}
}

void UTradeHandler::OnTradeStart()
{
}

void UTradeHandler::OnTradeFinish()
{
}

void UTradeHandler::OnTradeCancel(UObject* Caller)
{
	const int32 FoundIndex = FindIncomingTradeRequest(Caller);
	if(IncomingTradeRequests.IsValidIndex(FoundIndex))
	{
		IncomingTradeRequests.RemoveAt(FoundIndex);
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, IncomingTradeRequests, this);
		OnRep_IncomingTradeRequests();
	}
}
