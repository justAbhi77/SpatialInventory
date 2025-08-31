//

#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryPlugin.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "Net/UnrealNetwork.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "GameFramework/Pawn.h"

UInv_InventoryComponent::UInv_InventoryComponent() : InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	// Enables fast array subobject replication
	bReplicateUsingRegisteredSubObjectList = true;
	bInventoryMenuOpen = false;
}

void UInv_InventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, InventoryList);
}

void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	ConstructInventory();
}

void UInv_InventoryComponent::ConstructInventory()
{
	// Find and cache owning Player controller
	OwningController = Cast<APlayerController>(GetOwner());

#if WITH_EDITOR
	// Editor-only: Alert and log if attached to the wrong owner(not a PlayerController)
	if(!OwningController.IsValid())
	{
		if(const AActor* Owner = GetOwner(); Owner && Owner->HasAuthority())
		{
			auto MessageLog = FMessageLog(MESSAGE_LOG_LISTING);
			MessageLog.Error(FText::FromString(FString::Printf(TEXT("Actor %s should not have the Inventory Component. Only Player Controllers should have this component."), *Owner->GetClass()->GetName())));

			TSharedPtr<FDelegateHandle> ShutdownMessage = MakeShared<FDelegateHandle>();
			*ShutdownMessage = FEditorDelegates::ShutdownPIE.AddLambda([ShutdownMessage](bool bPlayInEditor){
				FMessageLog(MESSAGE_LOG_LISTING).Notify(FText::FromString("Inventory Component Error"), EMessageSeverity::Error, true);
				FEditorDelegates::ShutdownPIE.Remove(*ShutdownMessage);
			});
		}
	}
#endif // WITH_EDITOR

	if(!OwningController.IsValid() || !OwningController->IsLocalController())
		return;

	// Create the inventory menu widget and add it to the viewport(but hide by default)
	InventoryMenu = CreateWidget<UInv_InventoryBase>(OwningController.Get(), InventoryMenuClass);
	if(InventoryMenu)
	{
		InventoryMenu->AddToViewport();
		CloseInventoryMenu();
	}
}

void UInv_InventoryComponent::ToggleInventoryMenu()
{
	if(bInventoryMenuOpen)
		CloseInventoryMenu();
	else
		OpenInventoryMenu();

	OnInventoryMenuToggled.Broadcast(bInventoryMenuOpen);
}

void UInv_InventoryComponent::OpenInventoryMenu()
{
	if(!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;

	if(!OwningController.IsValid()) return;

	const FInputModeGameAndUI InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(true);
}

void UInv_InventoryComponent::CloseInventoryMenu()
{
	if(!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryMenuOpen = false;

	if(!OwningController.IsValid()) return;

	const FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(false);
}

void UInv_InventoryComponent::TryAddItem(UInv_ItemComponent* ItemComponent)
{
	// Check if there is space for the item(handles max stack and availability checks)
	FInv_SlotAvailabilityResult Result = InventoryMenu->HasRoomForItem(ItemComponent);
	UInv_InventoryItem* FoundItem = InventoryList.FindFirstItemByType(ItemComponent->GetItemManifest().GetItemType());
	Result.Item = FoundItem;

	if(Result.TotalRoomToFill == 0)
	{
		// No room: notify listeners/UI
		NoRoomInInventory.Broadcast();
		return;
	}

	if(Result.Item.IsValid() && Result.bStackable)
	{
		// Stacking onto an existing item: add stacks
		OnStackChange.Broadcast(Result);
		Server_AddStacksToItem(ItemComponent, Result.TotalRoomToFill, Result.Remainder);
	}
	else if(Result.TotalRoomToFill > 0)
	{
		// New item: add to inventory
		Server_AddNewItem(ItemComponent, Result.bStackable ? Result.TotalRoomToFill : 0, Result.Remainder);
	}
}

void UInv_InventoryComponent::Server_AddNewItem_Implementation(UInv_ItemComponent* ItemComponent, const int32 StackCount, const int32 Remainder)
{
	// Add a new inventory item entry for this component
	UInv_InventoryItem* NewItem = InventoryList.AddEntry(ItemComponent);
	NewItem->SetTotalStackCount(StackCount);

	// Notify listeners immediately if local/server
	if(GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
		OnItemAdded.Broadcast(NewItem);

	// Consume or leave the remainder in component in world
	if(Remainder == 0)
		ItemComponent->PickedUp();
	else if(FInv_StackableFragment* StackableFragment = ItemComponent->GetItemManifestMutable().GetFragmentOfTypeMutable<FInv_StackableFragment>())
		StackableFragment->SetStackCount(Remainder);
}

void UInv_InventoryComponent::Server_AddStacksToItem_Implementation(UInv_ItemComponent* ItemComponent, const int32 StackCount, const int32 Remainder)
{
	const FGameplayTag& ItemType = IsValid(ItemComponent) ? ItemComponent->GetItemManifest().GetItemType() : FGameplayTag::EmptyTag;
	UInv_InventoryItem* Item = InventoryList.FindFirstItemByType(ItemType);
	if(!IsValid(Item)) return;

	// Update stack count
	Item->SetTotalStackCount(Item->GetTotalStackCount() + StackCount);

	if(Remainder == 0)
		ItemComponent->PickedUp();
	else if(FInv_StackableFragment* StackableFragment = ItemComponent->GetItemManifestMutable().GetFragmentOfTypeMutable<FInv_StackableFragment>())
		StackableFragment->SetStackCount(Remainder);
}

void UInv_InventoryComponent::AddRepSubObj(UObject* SubObj)
{
	if(IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
		AddReplicatedSubObject(SubObj);
	else
		UE_LOG(LogActor, Error, TEXT("Inventory entry for an item could not be added to replication list."));
}

void UInv_InventoryComponent::Server_DropItem_Implementation(UInv_InventoryItem* Item, const int32 StackCount)
{
	// Remove or reduce stack, then spawn the dropped item actor
	if(const int32 NewStackCount = Item->GetTotalStackCount() - StackCount; NewStackCount <= 0)
		InventoryList.RemoveEntry(Item);
	else
		Item->SetTotalStackCount(NewStackCount);

	SpawnDroppedItem(Item, StackCount);
}

void UInv_InventoryComponent::SpawnDroppedItem(UInv_InventoryItem* Item, const int32 StackCount) const
{
	// Handle spawning dropped pickup in the world with randomization of location

	const APawn* OwningPawn = OwningController->GetPawn();
	FVector RotatedForward = OwningPawn->GetActorForwardVector();
	RotatedForward = RotatedForward.RotateAngleAxis(FMath::FRandRange(DropSpawnAngleMin, DropSpawnAngleMax), FVector::UpVector);

	FVector SpawnLocation = OwningPawn->GetActorLocation() + RotatedForward * FMath::FRandRange(DropSpawnDistanceMin, DropSpawnDistanceMax);
	SpawnLocation.Z -= RelativeSpawnDepression;
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	// Have the Item Manifest spawn the pickup actor.
	FInv_ItemManifest& ItemManifest = Item->GetItemManifestMutable();

	// Update stack count fragment with new value before spawning
	if(FInv_StackableFragment* StackableFragment = ItemManifest.GetFragmentOfTypeMutable<FInv_StackableFragment>())
		StackableFragment->SetStackCount(StackCount);

	ItemManifest.SpawnPickupActor(this, SpawnLocation, SpawnRotation);
}

void UInv_InventoryComponent::Server_ConsumeItem_Implementation(UInv_InventoryItem* Item, const int32 StackCount)
{
	// Remove or reduce stack and trigger item-specific consume functionality
	if(const int32 NewStackCount = Item->GetTotalStackCount() - StackCount; NewStackCount <= 0)
		InventoryList.RemoveEntry(Item);
	else
		Item->SetTotalStackCount(NewStackCount);

	// Call OnConsume on the item's consumable fragment, if one exists
	if(FInv_ConsumableFragment* ConsumableFragment = Item->GetItemManifestMutable().GetFragmentOfTypeMutable<FInv_ConsumableFragment>())
		ConsumableFragment->OnConsume(OwningController.Get(), StackCount);
}

void UInv_InventoryComponent::Server_EquipSlotClicked_Implementation(UInv_InventoryItem* ItemToEquip, UInv_InventoryItem* ItemToUnequip)
{
	// Broadcast the slot change to all network clients
	Multicast_EquipSlotClicked(ItemToEquip, ItemToUnequip);
}

void UInv_InventoryComponent::Multicast_EquipSlotClicked_Implementation(UInv_InventoryItem* ItemToEquip, UInv_InventoryItem* ItemToUnequip)
{
	// Notify listeners of equip/unequip(typically for equipment component)
	OnItemEquipped.Broadcast(ItemToEquip);
	OnItemUnequipped.Broadcast(ItemToUnequip);
}
