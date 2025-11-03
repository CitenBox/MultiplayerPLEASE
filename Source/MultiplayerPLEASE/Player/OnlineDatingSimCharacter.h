// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MultiplayerPLEASE/EGTypes.h"
#include "MultiplayerPLEASE/Interface/Interactable.h"
#include "GameFramework/Actor.h"
#include "MultiplayerPLEASE/Interface/ItemTransfer.h"
#include "MultiplayerPLEASE/Interface/Trader.h"
#include "MultiplayerPLEASE/Interface/HasPhysicsHandle.h"
#include "MultiplayerPLEASE/EGTypes.h"


#include "OnlineDatingSimCharacter.generated.h"


class UItemInfoDataAsset;
class UItemDataSubsystem;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class USphereComponent;
class UInventory;
struct FInputActionValue;
class AOnlineDatingCharacterController;
class UEnhancedInputLocalPlayerSubsystem;
class UOnlineDatingSimGameMode;
class ARagdoll;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemEquipped, EItemType)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemUnEquipped, EItemType)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemAction1, EItemType)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemAction2, EItemType)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemReload, EItemType)
DECLARE_MULTICAST_DELEGATE(FOnCharacterDeath)


/** Follow camera */
UCLASS(config=Game)
class MULTIPLAYERPLEASE_API AOnlineDatingSimCharacter : public ACharacter,public IInteractable,
public IItemTransfer, public ITrader, public IHasPhysicsHandle
{
	GENERATED_BODY()

	friend class AOnlineDatingCharacterController;



	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USphereComponent* InteractSphere;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PickupTestClass = nullptr;
	
	
	AOnlineDatingSimCharacter();

	virtual void BeginDestroy() override;

	FOnItemEquipped OnItemEquipped;
	FOnItemUnEquipped OnItemUnEquipped;
	FOnItemAction1 OnItemAction1;
	FOnItemAction2 OnItemAction2;
	FOnItemReload OnitemReload;
	FOnCharacterDeath OnCharacterDeath;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
protected:
	
	//input functions

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UPROPERTY()
	AOnlineDatingCharacterController* PlayerControllerRef = nullptr;
    
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem;
    	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FirstPerson, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonArms = nullptr;

	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandleComp;

	UPROPERTY()
	USphereComponent*  InteractRangeSphere;

	void EquipItem(int32 ItemIndex);
	UFUNCTION(Server, reliable)
	void EquipItemRPCServer(int32 ItemIndex);

	void UnEquipItem();
	UFUNCTION(Server, reliable)
	void UnEquipItemRPCServer();

	void DropItem();
	UFUNCTION(Server, reliable)
	void DropItemRPCServer();

	FORCEINLINE void DeleteHeldItemInstance();

protected:
	// APawn interface
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;

	virtual void Restart() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaSeconds) override;

public:

	
	void StartInteract();
	
	void PrimaryAction(bool bActive) const;
	void SecondaryAction(bool bActive) const;
	
	void StartRunning();
	void StopRunning();

	void Crouch();
	void UnCrouch();


	UFUNCTION()
	void HotbarInput(int32 HotbarNumber);

	virtual int32 GiveItem(const FEGItemInfo& Item) override;

	FORCEINLINE virtual UPhysicsHandleComponent* GetPhysicsHandleComponent() const override {return PhysicsHandleComp;}

	UFUNCTION()
	void OnSlotPressed(UObject* SlotRef, int32 Index);
	UFUNCTION(Server, Reliable)
	void OnSlotPressedRPCServer(int32 Index);

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Inventory")
	UInventory* PlayerInventoryRef = nullptr;

	UPROPERTY()
	FEGItemInfo EquippedItemInfo;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	UItemBase* EquippedItemInstance = nullptr;

	UPROPERTY(ReplicatedUsing=OnRep_EquippedItemID)
	uint16 EquippedItemID = 0;
	UFUNCTION()
	void OnRep_EquippedItemID();

	void SetEquippedItemVisibility(bool bVisible);

	UPROPERTY(EditAnywhere, Category = "Item Settings")
	float ItemDropRelativeDistance = 100.f;

	UPROPERTY(EditAnywhere, Category = "Item Settings")
	UItemInfoDataAsset* ItemInfoDataAsset = nullptr;

	UPROPERTY(EditAnywhere, Category = "Item Settings")
	FName ItemAttachmentSocketName = "weapon_rSocket";
	
	UPROPERTY(EditAnywhere, Category = "Item Settings")
	FName ItemAttachmentSocketNameFP = "GripPoint";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Settings", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ItemSkeletalMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Settings", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ItemSkeletalMeshComponentFP;
	
#ifdef WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif



	UPROPERTY(EditAnywhere, Category = "Stats")
	float MaxHealth = 100.f;
	UPROPERTY(ReplicatedUsing=OnRep_HealthChanged)
	float Health = 100.f;
	UFUNCTION()
	void OnRep_HealthChanged();

	void AddHealth(float AddBy);
	void RemoveHealth(float RemoveBy);

	void CheckDeath();
	
	bool bIsDying = false;

	UFUNCTION(NetMulticast, Unreliable)
	void OnDeath();

	UFUNCTION()
	void FinishDeath();
	

    UPROPERTY(EditAnywhere, Category="Death")
	TSubclassOf<ARagdoll> RagdollClass = nullptr;

	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY(EditAnywhere, Category="Movement")
	float MaxRunningSpeed = 700.f;

	UPROPERTY(VisibleAnywhere, Category="Movement")
	bool bIsRunning = false;

	UPROPERTY(EditAnywhere, Category="Movement")
	float MaxWalkSpeed = 500.f;

	virtual void Jump() override;
	virtual void StopJumping() override;

	virtual void Crouch(bool bClientSimulation) override;
	virtual void UnCrouch(bool bClientSimulation) override;
	
	UFUNCTION()
	void InteractOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void InteractOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor,float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);


	virtual void Interact(UObject* Caller) override;

	//trading

	UPROPERTY(Replicated)
	UTradeHandler* TradeHandler = nullptr;
	

	UFUNCTION()
	void OnTradeResponseHandle(UObject* Trader, bool bAccepted);
	
	virtual FString GetTraderName() override;

	virtual int32 GetTraderID() override;

};

