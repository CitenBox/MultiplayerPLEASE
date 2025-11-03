// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryUI.h"

#include "InventorySlotUI.h"
#include "Components/WrapBox.h"


void UInventoryUI::UpdateUI(const TArray<FEGItemInfo>& ItemArrayRef)
{
	for(int32 i = 0; i < InventoryWrapBox->GetChildrenCount(); i++)
	{
		const UInventorySlotUI* Temp = Cast<UInventorySlotUI>(InventoryWrapBox->GetChildAt(i));
		
		if(!ItemArrayRef.IsValidIndex(i + IndexStartOffset) || HideIndex == i + IndexStartOffset)
		{
			Temp->ItemSlotImage->SetVisibility(ESlateVisibility::Hidden);
			Temp->ItemSlotText->SetVisibility(ESlateVisibility::Hidden);
			continue;
		}

		if(ItemArrayRef[i+IndexStartOffset].ID == 0)
		{
			Temp->ItemSlotImage->SetVisibility(ESlateVisibility::Hidden);
			Temp->ItemSlotText->SetVisibility(ESlateVisibility::Hidden);
			continue;
		}
		
		Temp->ItemSlotImage->SetBrushFromTexture(ItemArrayRef[i + IndexStartOffset].Icon, true);
		Temp->ItemSlotText->SetText(FText::FromString(FString::FromInt(ItemArrayRef[i + IndexStartOffset].CurrentStack)));
		Temp->ItemSlotImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Temp->ItemSlotText->SetVisibility(ItemArrayRef[i + IndexStartOffset].CurrentStack <= 1 ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	}
}

void UInventoryUI::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	
}

void UInventoryUI::HandleSlotPressed(UObject* SlotRef, int32 SlotIndex)
{
	//UE_LOG(LogTemp, Log, TEXT("Inventory item slot ui at index %d was pressed!"), SlotIndex);
	OnInventorySlotPressed.Broadcast(SlotRef, SlotIndex);
}

void UInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();

	for (int32 i = 0; i < SlotAmount; ++i)
	{
		UInventorySlotUI* Temp = CreateWidget<UInventorySlotUI>(this, SlotClass);
		Temp->SlotIndex = i + IndexStartOffset;
		Temp->OnSlotPressedDelegate.BindUObject(this, &UInventoryUI::HandleSlotPressed);
		InventoryWrapBox->AddChildToWrapBox(Temp);
	}
}
