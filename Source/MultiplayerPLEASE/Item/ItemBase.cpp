// Fill out your copyright notice in the Description page of Project Settings.



#include "ItemBase.h"

#include <functional>


#include "Engine/EngineTypes.h"
#include "GameFramework/Character.h"
#include "UObject/WeakFieldPtr.h"



UItemBase::~UItemBase()
{
	UItemBase::DestroyItem();
}

void UItemBase::Initialize(FEGItemInfo& ItemInfo)
{
 	ItemInfoPtr = MakeShared<FEGItemInfo>(ItemInfo);
}

void UItemBase::Initialize(FEGItemInfo& ItemInfo, ACharacter* OuterCharacter, APlayerController* OuterPlayerController)
{
	Initialize(ItemInfo);

	OuterCharacterRef = OuterCharacter;
	OuterPlayerControllerRef = OuterPlayerController;

	if(!OuterCharacter || !OuterPlayerController)
	{
		return;
	}
	
	DoneInitiliaze();
}


void UItemBase::Action1(bool bActivate)
{
	static bool Toggle = false;
	
	if(Toggle)
	{
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Item action1 activated!"));
}

void UItemBase::Action2(bool bActivate)
{
	static bool Toggle = false;
	if(Toggle) {return;}
	
	UE_LOG(LogTemp, Log, TEXT("Item action2 activated!"));
}

void UItemBase::DestroyItem()
{
}

