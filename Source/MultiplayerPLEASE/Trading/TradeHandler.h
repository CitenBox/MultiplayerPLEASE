// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TradeHandler.generated.h"

class UInventory;
//class ITrader;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIncomingTradeRequest, UObject* , Trader);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateIncomingTradeRequests, const TArray<UObject*>& , TradeRequests);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERPLEASE_API UTradeHandler : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	UObject* ThisTraderRef = nullptr;
	UPROPERTY()
	UObject* OtherTraderRef = nullptr;

	//for when having a live trade session
	//we store the trade handler of the other player
	TWeakObjectPtr<UTradeHandler> OtherTradeHandler;

	UPROPERTY(Replicated)
	UInventory* ThisTradeInventory = nullptr;

	UPROPERTY(Replicated)
	UInventory* OtherTradeInventory = nullptr;

	UPROPERTY()
	UInventory* ThisPlayerInventoryRef = nullptr;

	virtual void BeginDestroy() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void StartTrade();

	virtual void BeginPlay() override;

public:

	FOnIncomingTradeRequest OnIncomingTradeRequest;
	FOnUpdateIncomingTradeRequests OnUpdateIncomingTradeRequests;
	
	// Sets default values for this component's properties
	UTradeHandler();

	virtual void InitTradeHandler(UObject* ThisTrader, UObject* OtherTrader, UInventory* ThisInventory);
	
	virtual void CancelTrade();

	//Array of trader pointers 
	UPROPERTY(ReplicatedUsing=OnRep_IncomingTradeRequests)
	TArray<UObject*> IncomingTradeRequests;
	UFUNCTION()
	void OnRep_IncomingTradeRequests();

	FORCEINLINE int32 FindIncomingTradeRequest(UObject* Trader);
	FORCEINLINE int32 FindOutGoingTradeRequest(UObject* Trader);

	UPROPERTY()
	TArray<UObject*> OutGoingTradeRequests;

	//this should be called locally when we get user input on whether to accept or not
	void RespondToTrade(UObject* Trader, bool bAccepted);
	
	UFUNCTION(Server, Reliable)
	void OnTradeResponseAcceptedRPCServer(UObject* Trader);
	UFUNCTION(Server, Reliable)
	void OnTradeResponseDeclinedRPCServer(UObject* Trader);

	virtual void TradeRequest(UObject* Trader);
	virtual void OnTradeRequest(UObject* Caller);

	virtual void OnTradeResponse(UObject* Caller, bool bAccepted);

	virtual void OnTradeStart();

	virtual void OnTradeFinish();

	virtual void OnTradeCancel(UObject* Caller);

};
