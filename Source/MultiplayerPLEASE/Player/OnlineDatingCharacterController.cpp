// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineDatingCharacterController.h"

#include <filesystem>

#include "MultiplayerPLEASE/UI/MultiplayerHUD.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "OnlineDatingSimCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerPLEASE/ExampleGameMode/OnlineDatingSimGameMode.h"
#include "Net/UnrealNetwork.h"
#include "MultiplayerPLEASE/Inventory/Inventory.h"

#include "MultiplayerPLEASE/UI/InventoryUI.h"
#include "OnlineDatingSimCharacter.h"
#include "MultiplayerPLEASE/Chat/OnlineGameState.h"
#include "MultiplayerPLEASE/Inventory/Inventory.h"
#include "MultiplayerPLEASE/UI/ChatUI.h"
#include "MultiplayerPLEASE/UI/HeldItemCursor.h"
#include "MultiplayerPLEASE/UI/MainMenu.h"
#include "MultiplayerPLEASE/UI/TradeMenu.h"
#include "MultiplayerPLEASE/UI/TradeNotification.h"

AOnlineDatingCharacterController::AOnlineDatingCharacterController()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	PlayerInventory = CreateDefaultSubobject<UInventory>(TEXT("PlayerInventory"));
}

void AOnlineDatingCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(ToggleInventoryUIAction, ETriggerEvent::Started, this, &ThisClass::ToggleInventoryUI);
		EnhancedInputComponent->BindAction(ToggleMenuUIAction, ETriggerEvent::Started, this, &ThisClass::ToggleMainMenuUI);
		EnhancedInputComponent->BindAction(ToggleTradeMenuUIAction, ETriggerEvent::Started, this, &ThisClass::ToggleTradeMenuUI);
		EnhancedInputComponent->BindAction(OpenChatUIAction, ETriggerEvent::Started, this, &ThisClass::OpenChatUI);
		

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

		//interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Interact);

		EnhancedInputComponent->BindAction(Mouse1Action, ETriggerEvent::Started, this, &ThisClass::Mouse1Started);
		EnhancedInputComponent->BindAction(Mouse2Action, ETriggerEvent::Started, this, &ThisClass::Mouse2Started);
		EnhancedInputComponent->BindAction(Mouse1Action, ETriggerEvent::Completed, this, &ThisClass::Mouse1Completed);
		EnhancedInputComponent->BindAction(Mouse2Action, ETriggerEvent::Completed, this, &ThisClass::Mouse2Completed);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ThisClass::RunStarted);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ThisClass::RunCompleted);
		
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::CrouchStarted);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ThisClass::CrouchCompleted);
		
		EnhancedInputComponent->BindAction(Hotbar1Action, ETriggerEvent::Started, this, &ThisClass::Hotbar1Input);
		EnhancedInputComponent->BindAction(Hotbar2Action, ETriggerEvent::Started, this, &ThisClass::Hotbar2Input);
		EnhancedInputComponent->BindAction(Hotbar3Action, ETriggerEvent::Started, this, &ThisClass::Hotbar3Input);
		EnhancedInputComponent->BindAction(Hotbar4Action, ETriggerEvent::Started, this, &ThisClass::Hotbar4Input);
		EnhancedInputComponent->BindAction(Hotbar5Action, ETriggerEvent::Started, this, &ThisClass::Hotbar5Input);
		EnhancedInputComponent->BindAction(Hotbar6Action, ETriggerEvent::Started, this, &ThisClass::Hotbar6Input);

		EnhancedInputComponent->BindAction(DropItemAction, ETriggerEvent::Started, this, &ThisClass::DropItem);
	}
}

void AOnlineDatingCharacterController::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(GetWorld()))
	{
		GameModeRef = GetWorld()->GetAuthGameMode<AOnlineDatingSimGameMode>();
		GameStateRef = GetWorld()->GetGameState<AOnlineGameState>();
	}
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 99);
	}

	SetInputMode(FInputModeGameOnly());

	if(!IsLocalController())
	{
		return;
	}
	
	if(MainMenuClass)
	{
		MainMenu = NewObject<UMainMenu>(this, MainMenuClass);
	}
    
	if(HUDClass)
	{
		HUDInstance = NewObject<UMultiplayerHUD>(this, HUDClass);
	}

	if(TradeMenuClass)
	{
		TradeMenuInstance = NewObject<UTradeMenu>(this, TradeMenuClass);
	}

	if(IsValid(MainMenu))
	{
		MainMenu->SetVisibility(ESlateVisibility::Collapsed);
		MainMenu->AddToViewport(999);
	}
	
	if(IsValid(TradeMenuInstance))
	{
		TradeMenuInstance->SetVisibility(ESlateVisibility::Collapsed);
		TradeMenuInstance->AddToViewport(99);
	}

	if(!IsValid(HUDInstance))return;
	

	HUDInstance->AddToViewport(9);

	if(HUDInstance->InventoryUI)
	{
		HUDInstance->InventoryUI->OnInventorySlotPressed.AddUObject(this, &ThisClass::OnSlotPressed);
	}
                                                                                                                                                
	if(HUDInstance->HotbarUI)
	{
		HUDInstance->HotbarUI->OnInventorySlotPressed.AddUObject(this, &ThisClass::OnSlotPressed);
	}

	if(HUDInstance->ChatUI)
	{
		HUDInstance->ChatUI->OnMessageSend.AddUObject(this, &ThisClass::OnChatMessageSend);
	}
	
}


void AOnlineDatingCharacterController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(bShouldHeldItemWidgetFollowCursor && CursorWidget)
	{
		float MouseX, MouseY;
		GetMousePosition(MouseX, MouseY);
		CursorWidget->SetPositionInViewport(FVector2D(MouseX, MouseY) * HeldItemWidgetOffset);
	}
}

void AOnlineDatingCharacterController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	
	Params.bIsPushBased = true;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PlayerInventory, Params);
}



void AOnlineDatingCharacterController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PlayerInventory->OnInventoryUpdate.AddUObject(this, &ThisClass::OnInventoryUpdate);

	//OnNewPawn.AddLambda([this](APawn* NewPawn)
	//{
	//	CharacterRef = Cast<AOnlineDatingSimCharacter>(NewPawn);
	//	if(CharacterRef)
	//	{
	//		CharacterRef->OnCharacterDeath.AddUObject(this, &ThisClass::OnCharacterDeathHandle);
	//	}
	//});
}

void AOnlineDatingCharacterController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);

	CharacterRef = Cast<AOnlineDatingSimCharacter>(P);
	if(IsValid(CharacterRef))
	{
		CharacterRef->OnCharacterDeath.AddUObject(this, &ThisClass::OnCharacterDeathHandle);
	}
}

void AOnlineDatingCharacterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CharacterRef = Cast<AOnlineDatingSimCharacter>(InPawn);
	if(IsValid(CharacterRef))
	{
		CharacterRef->OnCharacterDeath.AddUObject(this, &ThisClass::OnCharacterDeathHandle);
	}
}

void AOnlineDatingCharacterController::ToggleInventoryUI(const FInputActionValue& Value)
{
	if(!IsValid(HUDInstance))
	{
		return;
	}
	
	ToggleMenuUI(HUDInstance->InventoryUI);
}

void AOnlineDatingCharacterController::ToggleMainMenuUI(const FInputActionValue& Value)
{
	ToggleMenuUI(MainMenu);
}

void AOnlineDatingCharacterController::ToggleTradeMenuUI(const FInputActionValue& Value)
{
	if(!IsValid(TradeMenuInstance))
	{
		return;
	}

	ToggleMenuUI(TradeMenuInstance);
}

void AOnlineDatingCharacterController::OpenChatUI(const FInputActionValue& Value)
{
	if(!HUDInstance)return;

	if(!HUDInstance->ChatUI)return;

	SetIgnoreLookInput(true);
	SetShowMouseCursor(true);

	HUDInstance->ChatUI->OpenChat();
	
	HUDInstance->ChatUI->OnFocusLost.AddLambda([this]
	{
		SetIgnoreLookInput(false);
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	});
}

bool AOnlineDatingCharacterController::ToggleMenuUI(UUserWidget* UIToToggle)
{
	if(!IsValid(UIToToggle))
	{
		return false;
	}
	
	if(UIToToggle->GetVisibility() == ESlateVisibility::Visible)
	{
		SetShowMouseCursor(false);
		UIToToggle->SetVisibility(ESlateVisibility::Collapsed);
		SetInputMode(FInputModeGameOnly());
		SetIgnoreLookInput(false);
		SetIgnoreMoveInput(false);
	}
	else
	{
		SetShowMouseCursor(true);
		UIToToggle->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameAndUI InputModeGameAndUI;
		InputModeGameAndUI.SetHideCursorDuringCapture(false);
		InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		SetInputMode(InputModeGameAndUI);
		SetIgnoreLookInput(true);
		SetIgnoreMoveInput(true);
	}

	return true;
}

void AOnlineDatingCharacterController::OnInventoryUpdate()
{
	if(HUDInstance == nullptr || CharacterRef == nullptr || PlayerInventory == nullptr)
	{
		return;
	}
	
	if(HUDInstance->InventoryUI)
	{
		HUDInstance->InventoryUI->UpdateUI(PlayerInventory->InventoryArray.Items);
	}

	if(HUDInstance->HotbarUI)
	{
		HUDInstance->HotbarUI->UpdateUI(PlayerInventory->InventoryArray.Items);
	}
}

void AOnlineDatingCharacterController::OnChatMessageSend(const FString& Message)
{
	SendChatMessageServerRPC(Message);
}

void AOnlineDatingCharacterController::SendChatMessageServerRPC_Implementation(const FString& Message)
{
	if(GameStateRef)
	{
		GameStateRef->SendMessage(this, Message);
	}
}

void AOnlineDatingCharacterController::OnReplicatedPlayerInventory()
{
	if(IsLocalController() && HUDInstance && HUDInstance->InventoryUI && HUDInstance->HotbarUI)
	{
		HUDInstance->InventoryUI->UpdateUI(PlayerInventory->InventoryArray.Items);
		HUDInstance->HotbarUI->UpdateUI(PlayerInventory->InventoryArray.Items);
	}
}

void AOnlineDatingCharacterController::Move(const FInputActionValue& Value)
{
	if(CharacterRef)
	{
		CharacterRef->Move(Value);
	}
}

void AOnlineDatingCharacterController::Look(const FInputActionValue& Value)
{
	if(CharacterRef)
	{
		CharacterRef->Look(Value);
	}
}

void AOnlineDatingCharacterController::Interact(const FInputActionValue& Value)
{
	InteractServerRPC_Implementation();
	if(!HasAuthority())
	{
		InteractServerRPC();
	}
}

void AOnlineDatingCharacterController::InteractServerRPC_Implementation()
{
	if(CharacterRef)
	{
		CharacterRef->StartInteract();
	}
}

void AOnlineDatingCharacterController::Mouse1Started(const FInputActionValue& Value)
{
	Mouse1StartedServerRPC_Implementation();
	if(!HasAuthority())
	{
		Mouse1StartedServerRPC();
	}
}
void AOnlineDatingCharacterController::Mouse1Completed(const FInputActionValue& Value)
{
	Mouse1CompletedServerRPC_Implementation();
	if(!HasAuthority())
	{
		Mouse1CompletedServerRPC();
	}
}

void AOnlineDatingCharacterController::Mouse2Started(const FInputActionValue& Value)
{
	Mouse2StartedServerRPC_Implementation();
	if(!HasAuthority())
	{
		Mouse2StartedServerRPC();
	}
}

void AOnlineDatingCharacterController::Mouse2Completed(const FInputActionValue& Value)
{
	Mouse2CompletedServerRPC_Implementation();
	if(!HasAuthority())
	{
		Mouse2CompletedServerRPC();
	}
}

void AOnlineDatingCharacterController::Mouse2StartedServerRPC_Implementation()
{
	if(CharacterRef)
	{
		CharacterRef->SecondaryAction(true);
	}
}

void AOnlineDatingCharacterController::Mouse2CompletedServerRPC_Implementation()
{
	if(CharacterRef)
	{
		CharacterRef->SecondaryAction(false);
	}
}
void AOnlineDatingCharacterController::Mouse1CompletedServerRPC_Implementation()
{
	if(CharacterRef)
	{
		CharacterRef->PrimaryAction(false);
	}
}

void AOnlineDatingCharacterController::Mouse1StartedServerRPC_Implementation()
{
	if(CharacterRef)
	{
		CharacterRef->PrimaryAction(true);
	}
}

void AOnlineDatingCharacterController::RunStartedServerRPC_Implementation()
{
	bIsRunningPressed = true;
	if(CharacterRef)
	{
		CharacterRef->StartRunning();
	}
}

void AOnlineDatingCharacterController::RunCompletedServerRPC_Implementation()
{
	bIsRunningPressed = false;
	if(CharacterRef)
	{
		CharacterRef->StopRunning();
	}
}

void AOnlineDatingCharacterController::RunStarted(const FInputActionValue& Value)
{
	RunStartedServerRPC_Implementation();
	if(!HasAuthority())
	{
		RunStartedServerRPC();
	}
}

void AOnlineDatingCharacterController::RunCompleted(const FInputActionValue& Value)
{
	RunCompletedServerRPC_Implementation();
	if(!HasAuthority())
	{
		RunCompletedServerRPC();
	}
}

void AOnlineDatingCharacterController::CrouchStarted(const FInputActionValue& Value)
{
	CrouchStartedServerRPC_Implementation();
	if(!HasAuthority())
	{
		CrouchStartedServerRPC();
	}
}

void AOnlineDatingCharacterController::CrouchCompleted(const FInputActionValue& Value)
{
	CrouchCompletedServerRPC_Implementation();
	if(!HasAuthority())
	{
		CrouchCompletedServerRPC();
	}
}

void AOnlineDatingCharacterController::CrouchStartedServerRPC_Implementation()
{
	if(CharacterRef)
	{
		CharacterRef->Crouch();
	}
}

void AOnlineDatingCharacterController::CrouchCompletedServerRPC_Implementation()
{
	if(CharacterRef)
	{
		CharacterRef->UnCrouch();
	}
}

void AOnlineDatingCharacterController::Jump()
{
	if(CharacterRef)
	{
		CharacterRef->Jump();
	}
}

void AOnlineDatingCharacterController::StopJumping()
{
	if(CharacterRef)
	{
		CharacterRef->StopJumping();
	}
}

void AOnlineDatingCharacterController::Hotbar1Input(const FInputActionValue& Value)
{
	if(CharacterRef)
	{
		CharacterRef->HotbarInput(0);
	}
}

void AOnlineDatingCharacterController::Hotbar2Input(const FInputActionValue& Value)
{
	if(CharacterRef)
	{
		CharacterRef->HotbarInput(1);
	}
}

void AOnlineDatingCharacterController::Hotbar3Input(const FInputActionValue& Value)
{
	if(CharacterRef)
	{
		CharacterRef->HotbarInput(2);
	}
}

void AOnlineDatingCharacterController::Hotbar4Input(const FInputActionValue& Value)
{
	if(CharacterRef)
	{
		CharacterRef->HotbarInput(3);
	}
}

void AOnlineDatingCharacterController::Hotbar5Input(const FInputActionValue& Value)
{
	if(CharacterRef)
	{
		CharacterRef->HotbarInput(4);
	}
}

void AOnlineDatingCharacterController::Hotbar6Input(const FInputActionValue& Value)
{
	if(CharacterRef)
	{
		CharacterRef->HotbarInput(5);
	}
}

void AOnlineDatingCharacterController::DropItem(const FInputActionValue& Value)
{
	if(CharacterRef)
	{
		CharacterRef->DropItem();
	}
}

void AOnlineDatingCharacterController::OnCharacterDeathHandle()
{
	if(GameModeRef)
	{
		GameModeRef->OnPlayerDeath(this);
	}
}

void AOnlineDatingCharacterController::AddHeldItemWidget()
{
	RemoveHeldItemWidgetOnly();
	CursorWidget = CreateWidget<UHeldItemCursor>(this, HeldItemCursorClass);
	FEGItemInfo HeldItem = PlayerInventory->GetConstRefAt(MouseHeldItemIndex);
	if(CursorWidget && HeldItem.ID != 0)
	{
		CursorWidget->AddToViewport(99);
		CursorWidget->CustomInitialize(HeldItem.Icon, HeldItem.CurrentStack);
		bShouldHeldItemWidgetFollowCursor = true;
		CursorWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	
}

void AOnlineDatingCharacterController::RemoveHeldItemWidget()
{
	MouseHeldItemIndex = -1;
	HUDInstance->HotbarUI->HideIndex = -1;
	HUDInstance->InventoryUI->HideIndex = -1;
	
	if(CursorWidget)
	{
		bShouldHeldItemWidgetFollowCursor = false;
		CursorWidget->Destruct();
		CursorWidget->RemoveFromParent();
		CursorWidget = nullptr;
	}
}

void AOnlineDatingCharacterController::RemoveHeldItemWidgetOnly()
{
	if(CursorWidget)
	{
		bShouldHeldItemWidgetFollowCursor = false;
		CursorWidget->Destruct();
		CursorWidget->RemoveFromParent();
		CursorWidget = nullptr;
	}
}

void AOnlineDatingCharacterController::OnSlotPressed(UObject* SlotRef, int32 Index)
{
	if(PlayerInventory == nullptr || !IsLocalController())
	{
		return;
	}
	
	Index =	FMath::Clamp(Index, 0, PlayerInventory->MaxItems);

	if(MouseHeldItemIndex == -1)
	{
		if(!PlayerInventory->IsValidItem(Index))
     	{
     		return;
     	}
		
		if(CharacterRef)
		{
			if(Index == CharacterRef->EquippedItemInfo.Index)
			{
				CharacterRef->UnEquipItem();
			}
		}

		
		if(HUDInstance && HUDInstance->HotbarUI && HUDInstance->InventoryUI)
		{
			if(HUDInstance->InventoryUI->IndexStartOffset >= Index)
			{
				HUDInstance->HotbarUI->HideIndex = Index;
				OnInventoryUpdate();
			}
			else
			{
				HUDInstance->InventoryUI->HideIndex = Index;
				OnInventoryUpdate();
			}
		}
		
		MouseHeldItemIndex = Index;
		AddHeldItemWidget();
	}
	else
	{
		if(HUDInstance && HUDInstance->HotbarUI && HUDInstance->InventoryUI && Index == MouseHeldItemIndex)
		{
			if(HUDInstance->InventoryUI->IndexStartOffset >= Index)
			{
				HUDInstance->HotbarUI->HideIndex = -1;
				OnInventoryUpdate();
			}
			else
			{
				HUDInstance->InventoryUI->HideIndex = -1;
				OnInventoryUpdate();
			}
			RemoveHeldItemWidget();
		}
		
		PlayerInventory->MoveItem(MouseHeldItemIndex,Index);
		if(!HasAuthority())
		{
			PlayerInventory->MoveItemServerRPC(MouseHeldItemIndex,Index);
		}


		if(!PlayerInventory->IsValidItem(MouseHeldItemIndex))
		{
			RemoveHeldItemWidget();
		}
	}
}

void AOnlineDatingCharacterController::TriggerTradeNotification()
{
	if(IsLocalPlayerController() && HUDInstance && TradeNotificationClass && HUDInstance->TradeNotificationSlot)
	{
		if(HUDInstance->TradeNotificationSlot->GetChildrenCount() >= MaxTradeNotificationsOnScreen)
		{
			if(UWidget* ChildWidget = HUDInstance->TradeNotificationSlot->GetChildAt(0))
			{
				ChildWidget->RemoveFromParent();
			}
		}
		
		HUDInstance->TradeNotificationSlot->AddChild(CreateWidget(this, TradeNotificationClass));
	}
}
