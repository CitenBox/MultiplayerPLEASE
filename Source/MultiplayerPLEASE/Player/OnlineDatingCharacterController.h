// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Logging/LogMacros.h"
#include "MultiplayerPLEASE/EGTypes.h"

#include "OnlineDatingCharacterController.generated.h"

class AOnlineGameState;
class UInventory;
class UInventoryUI;
class UInputMappingContext;
class UInputAction;
class UMultiplayerHUD;
struct FInputActionValue;
class AOnlineDatingSimGameMode;
class UHeldItemCursor;
class UMainMenu;
class UTradeNotification;
class UTradeMenu;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemEquipped, EItemType)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemUnEquipped, EItemType)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemAction1, EItemType)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemAction2, EItemType)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemReload, EItemType)

UCLASS()
class MULTIPLAYERPLEASE_API AOnlineDatingCharacterController : public APlayerController
{
	GENERATED_BODY()

	//for debugging purposes
	ENetRole CurrentNetRole = ENetRole::ROLE_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleInventoryUIAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleMenuUIAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleTradeMenuUIAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenChatUIAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	//primary and secondary input actions
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Mouse1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Mouse2Action;

	//Hotbar input actions
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Hotbar1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Hotbar2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Hotbar3Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Hotbar4Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Hotbar5Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Hotbar6Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DropItemAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
	
public:

	FOnItemEquipped OnItemEquipped;
	FOnItemUnEquipped OnItemUnEquipped;
	FOnItemAction1 OnItemAction1;
	FOnItemAction2 OnItemAction2;
	FOnItemReload OnItemReload;

	AOnlineDatingCharacterController();

	virtual void PostInitializeComponents() override;

	UPROPERTY()
	AOnlineDatingSimGameMode* GameModeRef = nullptr;
	
protected:

	virtual void AcknowledgePossession(APawn* P) override;

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY()
	class AOnlineDatingSimCharacter* CharacterRef = nullptr;

	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void ToggleInventoryUI(const FInputActionValue& Value);

	void ToggleMainMenuUI(const FInputActionValue& Value);

	void ToggleTradeMenuUI(const FInputActionValue& Value);

	void OpenChatUI(const FInputActionValue& Value);

	FORCEINLINE bool ToggleMenuUI(UUserWidget* UIToToggle);

	UFUNCTION()
	void OnInventoryUpdate();

	UFUNCTION()
	void OnChatMessageSend(const FString& Message);

	UFUNCTION(Server, Reliable)
	void SendChatMessageServerRPC(const FString& Message);

	UPROPERTY()
	AOnlineGameState* GameStateRef;

	//deal with net relevancy conundrum (trollzinga)

	UFUNCTION()
	void OnReplicatedPlayerInventory();

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);
	UFUNCTION(Server, Reliable)
	void InteractServerRPC();
	
	void Mouse1Started(const FInputActionValue& Value);
	void Mouse1Completed(const FInputActionValue& Value);
	void Mouse2Started(const FInputActionValue& Value);
	void Mouse2Completed(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void Mouse1StartedServerRPC();

	UFUNCTION(Server, Reliable)
	void Mouse1CompletedServerRPC();

	UFUNCTION(Server, Reliable)
	void Mouse2StartedServerRPC();

	UFUNCTION(Server, Reliable)
	void Mouse2CompletedServerRPC();

	//Movement actions
	void RunStarted(const FInputActionValue& Value);
	void RunCompleted(const FInputActionValue& Value);
	UFUNCTION(Reliable, Server)
	void RunStartedServerRPC();
	UFUNCTION(Reliable, Server)
	void RunCompletedServerRPC();
	
	void CrouchStarted(const FInputActionValue& Value);
	void CrouchCompleted(const FInputActionValue& Value);
	UFUNCTION(Reliable, Server)
	void CrouchStartedServerRPC();
	UFUNCTION(Reliable, Server)
	void CrouchCompletedServerRPC();
	
	virtual void Jump();
	virtual void StopJumping();

	//hotbar actions
	
	void Hotbar1Input(const FInputActionValue& Value);
	void Hotbar2Input(const FInputActionValue& Value);
	void Hotbar3Input(const FInputActionValue& Value);
	void Hotbar4Input(const FInputActionValue& Value);
	void Hotbar5Input(const FInputActionValue& Value);
	void Hotbar6Input(const FInputActionValue& Value);

	void DropItem(const FInputActionValue& Value);

	public:

	UFUNCTION()
	void OnCharacterDeathHandle();

	UPROPERTY(VisibleAnywhere, Category="Movement")
	bool bIsRunningPressed = false;

	UPROPERTY(EditInstanceOnly ,ReplicatedUsing = OnInventoryUpdate)
	UInventory* PlayerInventory;

	UPROPERTY()
	UMainMenu* MainMenu = nullptr;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UMainMenu> MainMenuClass = nullptr;

	UPROPERTY()
	UMultiplayerHUD* HUDInstance = nullptr;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UMultiplayerHUD> HUDClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "UI|Inventory")
	TSubclassOf<UUserWidget> HeldItemCursorClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "UI|Inventory")
	FVector2D HeldItemWidgetOffset;

	bool bShouldHeldItemWidgetFollowCursor = false;
	
	UFUNCTION()
	void AddHeldItemWidget();
	UFUNCTION()
	void RemoveHeldItemWidget();
	UFUNCTION()
	void RemoveHeldItemWidgetOnly();

	UFUNCTION()
	void OnSlotPressed(UObject* SlotRef, int32 Index);

	UPROPERTY()
	UHeldItemCursor* CursorWidget = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int32 MouseHeldItemIndex = -1;

	//trading

	UPROPERTY(EditANywhere, Category="UI|Trading")
	TSubclassOf<UTradeMenu> TradeMenuClass = nullptr;

	UPROPERTY()
	UTradeMenu* TradeMenuInstance = nullptr;

	UPROPERTY(EditAnywhere, Category="UI|Trading")
	TSubclassOf<UTradeNotification> TradeNotificationClass = nullptr;

	void TriggerTradeNotification();

	UPROPERTY(EditAnywhere, Category="UI|Trading")
	int32 MaxTradeNotificationsOnScreen = 5;
	
};
