// Copyright Epic Games, Inc. All Rights Reserved.

#include "OnlineDatingSimCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"
#include "MultiplayerPLEASE/Interface/Interactable.h"
#include "MultiplayerPLEASE/Inventory/Inventory.h"
#include "MultiplayerPLEASE/Item/ItemBase.h"
#include "OnlineDatingCharacterController.h"
#include "MultiplayerPLEASE/UI/MultiplayerHUD.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerState.h"
#include "MultiplayerPLEASE/UI/HealthUI.h"
#include "MultiplayerPLEASE/Player/Ragdoll/Ragdoll.h"
#include "MultiplayerPLEASE/Trading/TradeHandler.h"
#include "MultiplayerPLEASE/UI/TradeMenu.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "DrawDebugHelpers.h"
#include "MultiplayerPLEASE/Item/ItemInfoDataAsset.h"
#include "MultiplayerPLEASE/Pickup/BasePickup.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AOnlineDatingSimCharacter

AOnlineDatingSimCharacter::AOnlineDatingSimCharacter()
{
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphere"));
	InteractSphere->SetupAttachment(RootComponent);
	InteractSphere->SetRelativeScale3D(FVector(2));
	InteractSphere->SetGenerateOverlapEvents(true);
	InteractSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	InteractSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Overlap);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	//GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Linear;
	
	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed; 
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 0.f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = 0;
	CameraBoom->SetRelativeLocation(FVector(0.f,0.f,65.f));// Rotate the arm based on the controller
	

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	FirstPersonArms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonArms"));
	FirstPersonArms->SetupAttachment(FollowCamera);
	FirstPersonArms->SetRelativeLocation(FVector(-10.f,0.f,-155.f));
	FirstPersonArms->CastShadow = false;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	PhysicsHandleComp = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandleComponent"));
	
	ItemSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMeshComponent"));
	ItemSkeletalMeshComponent->SetupAttachment(GetMesh());
	ItemSkeletalMeshComponent->SetOwnerNoSee(true);
	ItemSkeletalMeshComponent->SetIsReplicated(false);
	
	ItemSkeletalMeshComponentFP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMeshComponentFP"));
	ItemSkeletalMeshComponentFP->SetupAttachment(FirstPersonArms);
	ItemSkeletalMeshComponentFP->SetOnlyOwnerSee(true);
	ItemSkeletalMeshComponentFP->SetIsReplicated(false);

	CharacterMovementComponent = Cast<UCharacterMovementComponent>(ACharacter::GetMovementComponent());
}

void AOnlineDatingSimCharacter::BeginDestroy()
{
	Super::BeginDestroy();
}


void AOnlineDatingSimCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerControllerRef = Cast<AOnlineDatingCharacterController>(GetOwner());
	
	if (PlayerControllerRef)
	{
		EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControllerRef->GetLocalPlayer());
		if (EnhancedInputLocalPlayerSubsystem)
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(DefaultMappingContext, 9);
		}
	}

	if(InteractSphere)
	{
		InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &AOnlineDatingSimCharacter::InteractOverlapBegin);
		InteractSphere->OnComponentEndOverlap.AddDynamic(this, &AOnlineDatingSimCharacter::InteractOverlapEnd);
	}
	
	OnTakeAnyDamage.AddDynamic(this, &ThisClass::ReceiveDamage);
	
}

void AOnlineDatingSimCharacter::Restart()
{
	Super::Restart();

	

	PlayerControllerRef = Cast<AOnlineDatingCharacterController>(GetController());

	if(PlayerControllerRef)
	{
		PlayerInventoryRef = PlayerControllerRef->PlayerInventory;
	}
	
	if(IsLocallyControlled() && PlayerControllerRef)
	{
		if(PlayerControllerRef->TradeMenuInstance)
		{
			PlayerControllerRef->TradeMenuInstance->OnRequestResponse.AddDynamic(this, &ThisClass::OnTradeResponseHandle);
		}
	}
	
	OnRep_HealthChanged();
}


void AOnlineDatingSimCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	
	Params.bIsPushBased = true;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	Params.Condition = COND_None;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, Health, Params);
	
	Params.bIsPushBased = true;
	Params.RepNotifyCondition = REPNOTIFY_OnChanged;
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, TradeHandler, Params);
	
	Params.bIsPushBased = true;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	Params.Condition = COND_None;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, EquippedItemID, Params);
}

void AOnlineDatingSimCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AOnlineDatingSimCharacter::StartInteract()
{
	FHitResult Hit = FHitResult();
	FVector Start = FollowCamera->GetComponentLocation();
	FVector ForwardVec = FollowCamera->GetComponentRotation().Vector();
	FVector End = Start + (ForwardVec * 1000);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel
	(
		Hit,
		Start,
		End,
		ECC_GameTraceChannel1,
		Params
	);
	
	if (IInteractable* Interactable = Cast<IInteractable>(Hit.GetActor()))
	{
		Interactable->Interact(this);
	}

	if(Hit.GetActor() && Hit.GetActor()->Implements<UTrader>())
	{
		if(TradeHandler)
		{
			TradeHandler->TradeRequest(Hit.GetActor());
		}
	}
}

void AOnlineDatingSimCharacter::StartRunning()
{
	if(CharacterMovementComponent)
	{
		CharacterMovementComponent->MaxWalkSpeed = MaxRunningSpeed;
	}
	
}
void AOnlineDatingSimCharacter::StopRunning()
{
	if(CharacterMovementComponent)
	{
		CharacterMovementComponent->MaxWalkSpeed = MaxWalkSpeed;
	}
}

void AOnlineDatingSimCharacter::Crouch()
{
	if(CharacterMovementComponent)
	{
		CharacterMovementComponent->bWantsToCrouch = true;
		//CharacterMovementComponent->Crouch(!HasAuthority());
	}
}

void AOnlineDatingSimCharacter::UnCrouch()
{
	if(CharacterMovementComponent)
	{
		CharacterMovementComponent->bWantsToCrouch = false;
		//CharacterMovementComponent->UnCrouch(!HasAuthority());
	}
}

int32 AOnlineDatingSimCharacter::GiveItem(const FEGItemInfo& Item)
{
	if(PlayerInventoryRef)
	{
		return PlayerInventoryRef->AddItem(Item);
	}
	return -1;
}

void AOnlineDatingSimCharacter::OnSlotPressed(UObject* SlotRef, int32 Index)
{
	OnSlotPressedRPCServer_Implementation(Index);

	if(!HasAuthority())
	{
		OnSlotPressedRPCServer(Index);
	}
}

void AOnlineDatingSimCharacter::OnRep_EquippedItemID()
{
	if(EquippedItemInfo.ID != EquippedItemID)
 			EquippedItemInfo = ItemInfoDataAsset->GetItemInfo(EquippedItemID);
	
	if(EquippedItemInfo.ID != 0)
	{
		SetEquippedItemVisibility(true);
	}
	else
	{
		SetEquippedItemVisibility(false);
	}
	
}

void AOnlineDatingSimCharacter::SetEquippedItemVisibility(bool bVisible)
{
	if(!bVisible)
	{
		ItemSkeletalMeshComponent->SetVisibility(false);
		ItemSkeletalMeshComponentFP->SetVisibility(false);
		
		OnItemUnEquipped.Broadcast(EItemType::None);
	}
	else
	{
		if(!IsValid(EquippedItemInstance))
		{
			EquippedItemInstance = NewObject<UItemBase>(this, EquippedItemInfo.ItemImplementation);
		}
		
		ItemSkeletalMeshComponent->SetSkeletalMesh(EquippedItemInstance->ItemMesh);
		ItemSkeletalMeshComponent->SetVisibility(!IsLocallyViewed());
		
		ItemSkeletalMeshComponentFP->SetSkeletalMesh(EquippedItemInstance->ItemMeshFP);
		ItemSkeletalMeshComponentFP->SetVisibility(IsLocallyViewed());

		OnItemEquipped.Broadcast(EquippedItemInfo.ItemType);
	}
}

void AOnlineDatingSimCharacter::OnSlotPressedRPCServer_Implementation(int32 Index)
{
}

#ifdef WITH_EDITOR
void AOnlineDatingSimCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if(!IsValid(ItemSkeletalMeshComponent) || !IsValid(ItemSkeletalMeshComponentFP))
		return;

	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	
	ItemSkeletalMeshComponent->AttachToComponent(GetMesh(), AttachmentRules, ItemAttachmentSocketName);
	
	ItemSkeletalMeshComponentFP->AttachToComponent(FirstPersonArms, AttachmentRules, ItemAttachmentSocketNameFP);
}
#endif

void AOnlineDatingSimCharacter::OnRep_HealthChanged()
{
	if(IsLocallyControlled() && PlayerControllerRef && PlayerControllerRef->HUDInstance && PlayerControllerRef->HUDInstance->HealthUI)
	{
		PlayerControllerRef->HUDInstance->HealthUI->UpdateHealthUI(Health);
	}
}

void AOnlineDatingSimCharacter::AddHealth(float AddBy)
{
	Health += AddBy;
	Health = FMath::Clamp(Health, 0, MaxHealth);
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, Health, this);
	OnRep_HealthChanged();
}

void AOnlineDatingSimCharacter::RemoveHealth(float RemoveBy)
{
	AddHealth(-RemoveBy);
	CheckDeath();
}

void AOnlineDatingSimCharacter::CheckDeath()
{
	if(Health <= 0 && !bIsDying)
	{
		bIsDying = true;
		OnDeath();

		if(HasAuthority())
		{
			FinishDeath();
		}
	}
}

void AOnlineDatingSimCharacter::OnDeath_Implementation()
{
	if(RagdollClass)
	{
		if(ARagdoll* Ragdoll = GetWorld()->SpawnActor<ARagdoll>(RagdollClass, GetMesh()->GetComponentTransform()))
		{
			Ragdoll->StartRagdoll(this);
		}
	}
	
	SetActorHiddenInGame(true);
	Destroy();
}

void AOnlineDatingSimCharacter::FinishDeath()
{
	OnCharacterDeath.Broadcast();
	SetActorHiddenInGame(true);
 	Destroy();
}

void AOnlineDatingSimCharacter::Jump()
{
	Super::Jump();
	
	StopRunning();
}

void AOnlineDatingSimCharacter::StopJumping()
{
	Super::StopJumping();

	if(PlayerControllerRef && PlayerControllerRef->bIsRunningPressed)
	{
		StartRunning();
	}
}

void AOnlineDatingSimCharacter::Crouch(bool bClientSimulation)
{
	Super::Crouch(bClientSimulation);
}

void AOnlineDatingSimCharacter::UnCrouch(bool bClientSimulation)
{
	Super::UnCrouch(bClientSimulation);
}

void AOnlineDatingSimCharacter::InteractOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == this) { return; }

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("overlaped!")));
}

void AOnlineDatingSimCharacter::InteractOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("overlap ended!")));
}

void AOnlineDatingSimCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	RemoveHealth(Damage);
}

void AOnlineDatingSimCharacter::Interact(UObject* Caller)
{

}

void AOnlineDatingSimCharacter::OnTradeResponseHandle(UObject* Trader, bool bAccepted)
{
	if(TradeHandler)
	{
		TradeHandler->RespondToTrade(Trader, bAccepted);
	}
}

FString AOnlineDatingSimCharacter::GetTraderName()
{
	if(GetPlayerState())
	{
		return GetPlayerState()->GetPlayerName();
	}

	return GetName();
}

int32 AOnlineDatingSimCharacter::GetTraderID()
{
	if(GetPlayerState())
	{
		return GetPlayerState()->GetPlayerId();
	}

	return 0;
}

void AOnlineDatingSimCharacter::PrimaryAction(bool bActive) const
{
	if (EquippedItemInstance)
	{
		EquippedItemInstance->Action1(bActive);
	}
}

void AOnlineDatingSimCharacter::SecondaryAction(bool bActive) const
{
	if (EquippedItemInstance)
	{
		EquippedItemInstance->Action2(bActive);
	}
}


void AOnlineDatingSimCharacter::HotbarInput(int32 HotbarNumber)
{
	EquipItem(HotbarNumber);
}


void AOnlineDatingSimCharacter::EquipItem(int32 ItemIndex)
{
	if(!PlayerInventoryRef->InventoryArray.Items.IsValidIndex(ItemIndex)
		&&
		!PlayerInventoryRef->InventoryArray.Items[ItemIndex].ItemImplementation)
	{
		UnEquipItemRPCServer_Implementation();
		if(!HasAuthority())
		{
			UnEquipItemRPCServer();
		}
		return;
	}
	
	EquipItemRPCServer_Implementation(ItemIndex);
	if(!HasAuthority())
	{
		EquipItemRPCServer(ItemIndex);
	}
}


void AOnlineDatingSimCharacter::UnEquipItem()
{
	if(EquippedItemInstance)
	{
		UnEquipItemRPCServer_Implementation();

		if(!HasAuthority())
		{
			UnEquipItemRPCServer();
		}
	}
}

void AOnlineDatingSimCharacter::DropItem()
{
	DropItemRPCServer_Implementation();
	if(!HasAuthority())
	{
		DropItemRPCServer();
	}
}

//removes item from inventory and spawns a pickup actor with a specified offset
void AOnlineDatingSimCharacter::DropItemRPCServer_Implementation()
{
	if(GetWorld() && PlayerControllerRef)
	{
		if(HasAuthority())
		{
			FActorSpawnParameters SpawnParameters;
			FVector SpawnLocation = PlayerControllerRef->GetControlRotation().Vector();
			SpawnLocation.Normalize();
			SpawnLocation *= ItemDropRelativeDistance;
			SpawnLocation += GetActorLocation();
			GetWorld()->SpawnActor<ABasePickup>(EquippedItemInfo.ItemPickupClass, FTransform(SpawnLocation));
		}
			
		
		PlayerControllerRef->PlayerInventory->RemoveAt(EquippedItemInfo.Index);
		UnEquipItemRPCServer_Implementation();
	}
}

void AOnlineDatingSimCharacter::UnEquipItemRPCServer_Implementation()
{
	DeleteHeldItemInstance();
	EquippedItemID = 0;
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, EquippedItemID, this);
	//might need to set the visibility not from onRep. I forgot if it runs on the server and too lazy to check
	//SetEquippedItemVisibility(false);
	OnRep_EquippedItemID();
}

void AOnlineDatingSimCharacter::EquipItemRPCServer_Implementation(int32 ItemIndex)
{
	if(HasAuthority())
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Server!")));
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Client!")));
	
	ItemIndex = FMath::Clamp(ItemIndex, 0, PlayerInventoryRef->HotbarSlotNum);

	EquippedItemInfo = PlayerInventoryRef->InventoryArray.Items[ItemIndex];
	
	if(EquippedItemInfo.ItemImplementation == nullptr || EquippedItemInfo.ID == 0)
	{
		UnEquipItemRPCServer_Implementation();
		return;
	}
	
	DeleteHeldItemInstance();
	
	EquippedItemInstance = NewObject<UItemBase>(this, EquippedItemInfo.ItemImplementation);
	
	if(EquippedItemInstance)
	{
		EquippedItemInstance->Initialize(PlayerInventoryRef->InventoryArray.Items[ItemIndex], this, PlayerControllerRef);

		EquippedItemID = EquippedItemInfo.ID;
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, EquippedItemID, this);
		OnRep_EquippedItemID();
	}
	
}

void AOnlineDatingSimCharacter::DeleteHeldItemInstance()
{
	if(IsValid(EquippedItemInstance))
	{
		EquippedItemInstance->DestroyItem();
		EquippedItemInstance = nullptr;
	}
}

void AOnlineDatingSimCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		

		// Moving
		//EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

		// Looking
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AOnlineDatingSimCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AOnlineDatingSimCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
