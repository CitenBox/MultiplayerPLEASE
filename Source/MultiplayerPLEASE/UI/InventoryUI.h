// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerPLEASE/EGTypes.h"

#include "InventoryUI.generated.h"

class UInventorySlotUI;
class UWrapBox;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInventorySlotPressed, UObject*, int32);

UCLASS()
class MULTIPLAYERPLEASE_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()

	public:

	FOnInventorySlotPressed OnInventorySlotPressed;
	
	UPROPERTY(EditAnywhere)
	int32 IndexStartOffset = 6;

	UPROPERTY(EditAnywhere)
	int32 SlotAmount = 20;

	//for when you want to hide an item because it is being held by the mouse.
	UPROPERTY(EditAnywhere)
	int32 HideIndex = -1;
	
	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta=(BindWidget))
	UWrapBox* InventoryWrapBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventorySlotUI> SlotClass;

	void UpdateUI(const TArray<FEGItemInfo>& ItemArrayRef);

	void InitializeSlotsArray(const TArray<FEGItemInfo>& ItemArrayRef);

	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

	UFUNCTION()
	void HandleSlotPressed(UObject* SlotRef, int32 SlotIndex);

	void NativeConstruct() override;
	
};
