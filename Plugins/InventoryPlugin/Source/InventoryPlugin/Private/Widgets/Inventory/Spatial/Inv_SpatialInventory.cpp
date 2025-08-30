//

#include "Widgets/Inventory/Spatial/Inv_SpatialInventory.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UObject/Object.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "InventoryPlugin.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Components/CanvasPanel.h"
#include "Widgets/ItemDescription/Inv_ItemDescription.h"
#include "TimerManager.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Items/Inv_InventoryItem.h"
#include "Blueprint/WidgetTree.h"
#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlot.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"
#include "Widgets/Inventory/SlottedItems/Inv_EquippedSlottedItem.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Items/Fragments/Inv_ItemFragment.h"


void UInv_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Equippables->OnClicked.AddDynamic(this, &ThisClass::ShowEquippables);
	Button_Consumables->OnClicked.AddDynamic(this, &ThisClass::ShowConsumables);
	Button_Craftables->OnClicked.AddDynamic(this, &ThisClass::ShowCraftables);

	Grid_Equippables->SetOwningCanvas(CanvasPanel);
	Grid_Consumables->SetOwningCanvas(CanvasPanel);
	Grid_Craftables->SetOwningCanvas(CanvasPanel);

	ShowEquippables();

	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		if(UInv_EquippedGridSlot* EquippedGridSlot = Cast<UInv_EquippedGridSlot>(Widget); IsValid(EquippedGridSlot))
		{
			EquippedGridSlots.Add(EquippedGridSlot);
			EquippedGridSlot->EquippedGridSlotClicked.AddDynamic(this, &ThisClass::EquippedGridSlotClicked);
		}
	});
}

void UInv_SpatialInventory::ShowEquippables()
{
	SetActiveGrid(Grid_Equippables, Button_Equippables);
}

void UInv_SpatialInventory::ShowConsumables()
{
	SetActiveGrid(Grid_Consumables, Button_Consumables);
}

void UInv_SpatialInventory::ShowCraftables()
{
	SetActiveGrid(Grid_Craftables, Button_Craftables);
}

void UInv_SpatialInventory::DisableButton(UButton* Button) const
{
	Button_Equippables->SetIsEnabled(true);
	Button_Consumables->SetIsEnabled(true);
	Button_Craftables->SetIsEnabled(true);
	Button->SetIsEnabled(false);
}

void UInv_SpatialInventory::SetActiveGrid(UInv_InventoryGrid* Grid, UButton* Button)
{
	if(ActiveGrid.IsValid())
	{
		ActiveGrid->HideCursor();
		ActiveGrid->OnHide();
	}

	ActiveGrid = Grid;
	if(ActiveGrid.IsValid()) ActiveGrid->ShowCursor();

	DisableButton(Button);
	Switcher->SetActiveWidget(Grid);
}

FInv_SlotAvailabilityResult UInv_SpatialInventory::HasRoomForItem(UInv_ItemComponent* ItemComponent) const
{
	switch(UInv_InventoryStatics::GetItemCategoryFromItemComp(ItemComponent))
	{
		case EInv_ItemCategory::Equippable:
			return Grid_Equippables->HasRoomForItem(ItemComponent);
		case EInv_ItemCategory::Consumable:
			return Grid_Consumables->HasRoomForItem(ItemComponent);
		case EInv_ItemCategory::Craftable:
			return Grid_Craftables->HasRoomForItem(ItemComponent);
		default:
			UE_LOG(LogInventory, Error, TEXT("ItemComponent doesn't have a valid Item Category."))
			return FInv_SlotAvailabilityResult();
	}
}

FReply UInv_SpatialInventory::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	ActiveGrid->DropItem();
	return FReply::Handled();
}

void UInv_SpatialInventory::OnItemHovered(UInv_InventoryItem* Item)
{
	const auto& Manifest = Item->GetItemManifest();
	if(UInv_ItemDescription* DescriptionWidget = GetItemDescription())
		DescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);

	FTimerManager& TimerManager = GetOwningPlayer()->GetWorldTimerManager();
	TimerManager.ClearTimer(DescriptionTimer);
	TimerManager.ClearTimer(EquippedDescriptionTimer);

	FTimerDelegate DescriptionTimerDelegate;
	DescriptionTimerDelegate.BindLambda([this, &Manifest, Item]()
	{
		if(UInv_ItemDescription* DescriptionWidget = GetItemDescription())
		{
			// Assimilate the manifest into the item description widget
			DescriptionWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
			Manifest.AssimilateInventoryFragments(DescriptionWidget);

			// For the second item description, showing the equipped item of this type.
			FTimerDelegate EquippedDescriptionTimerDelegate;
			EquippedDescriptionTimerDelegate.BindUObject(this, &ThisClass::ShowEquippedItemDescription, Item);
			GetOwningPlayer()->GetWorldTimerManager().SetTimer(EquippedDescriptionTimer, EquippedDescriptionTimerDelegate, EquippedDescriptionTimerDelay, false);
		}
	});

	TimerManager.SetTimer(DescriptionTimer, DescriptionTimerDelegate, DescriptionTimerDelay, false);
}

void UInv_SpatialInventory::OnItemUnHovered()
{
	FTimerManager& TimerManager = GetOwningPlayer()->GetWorldTimerManager();
	TimerManager.ClearTimer(DescriptionTimer);
	TimerManager.ClearTimer(EquippedDescriptionTimer);

	if(UInv_ItemDescription* TempItemDescription = GetItemDescription())
		TempItemDescription->SetVisibility(ESlateVisibility::Collapsed);

	if(UInv_ItemDescription* EquippedItem = GetEquippedItemDescription())
		EquippedItem->SetVisibility(ESlateVisibility::Collapsed);
}

bool UInv_SpatialInventory::HasHoverItem() const
{
	if(Grid_Equippables->HasHoverItem()) return true;
	if(Grid_Consumables->HasHoverItem()) return true;
	if(Grid_Craftables->HasHoverItem()) return true;
	return false;
}

UInv_ItemDescription* UInv_SpatialInventory::GetItemDescription()
{
	if(!IsValid(ItemDescription) && IsValid(ItemDescriptionClass))
	{
		ItemDescription = CreateWidget<UInv_ItemDescription>(GetOwningPlayer(), ItemDescriptionClass);
		CanvasPanel->AddChild(ItemDescription);
	}
	return ItemDescription;
}

void UInv_SpatialInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!IsValid(ItemDescription)) return;
	SetItemDescriptionSizeAndPosition(ItemDescription, CanvasPanel);

	SetEquippedItemDescriptionSizeAndPosition(ItemDescription, EquippedItemDescription, CanvasPanel);
}

void UInv_SpatialInventory::SetItemDescriptionSizeAndPosition(UInv_ItemDescription* Description, UCanvasPanel* Canvas) const
{
	UCanvasPanelSlot* ItemDescriptionCps = UWidgetLayoutLibrary::SlotAsCanvasSlot(Description);
	if(!IsValid(ItemDescriptionCps)) return;

	const FVector2D ItemDescriptionSize = Description->GetBoxSize();
	ItemDescriptionCps->SetSize(ItemDescriptionSize);

	const FVector2D ClampedPosition = UInv_WidgetUtils::GetClampedWidgetPosition(UInv_WidgetUtils::GetWidgetSize(Canvas), ItemDescriptionSize, UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer()));

	ItemDescriptionCps->SetPosition(ClampedPosition);
}

void UInv_SpatialInventory::EquippedGridSlotClicked(UInv_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag)
{
	// Check to see if we can equip the Hover Item
	if(!CanEquipHoverItem(EquippedGridSlot, EquipmentTypeTag)) return;

	const UInv_HoverItem* HoverItem = GetHoverItem();

	// Create an Equipped Slotted Item and add it to the Equipped Grid Slot (call EquippedGridSlot->OnItemEquipped())
	const float TileSize = UInv_InventoryStatics::GetInventoryWidget(GetOwningPlayer())->GetTileSize();
	UInv_EquippedSlottedItem* EquippedSlottedItem = EquippedGridSlot->OnItemEquipped(HoverItem->GetInventoryItem(), EquipmentTypeTag, TileSize);
	EquippedSlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this, &ThisClass::EquippedSlottedItemClicked);

	// Inform the server that we've equipped an item (potentially unequipping an item as well)
	UInv_InventoryComponent* InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	check(IsValid(InventoryComponent));

	InventoryComponent->Server_EquipSlotClicked(HoverItem->GetInventoryItem(), nullptr);

	if(GetOwningPlayer()->GetNetMode() != NM_DedicatedServer)
		InventoryComponent->OnItemEquipped.Broadcast(HoverItem->GetInventoryItem());

	// Clear the Hover Item
	Grid_Equippables->ClearHoverItem();
}

UInv_HoverItem* UInv_SpatialInventory::GetHoverItem() const
{
	if(!ActiveGrid.IsValid()) return nullptr;
	return ActiveGrid->GetHoverItem();
}

bool UInv_SpatialInventory::CanEquipHoverItem(const UInv_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag) const
{
	if(!IsValid(EquippedGridSlot) || EquippedGridSlot->GetInventoryItem().IsValid()) return false;

	const UInv_HoverItem* HoverItem = GetHoverItem();
	if(!IsValid(HoverItem)) return false;

	const UInv_InventoryItem* HeldItem = HoverItem->GetInventoryItem();

	return HasHoverItem() && IsValid(HeldItem) && !HoverItem->IsStackable() &&
			HeldItem->GetItemManifest().GetItemCategory() == EInv_ItemCategory::Equippable &&
				HeldItem->GetItemManifest().GetItemType().MatchesTag(EquipmentTypeTag);
}

void UInv_SpatialInventory::EquippedSlottedItemClicked(UInv_EquippedSlottedItem* EquippedSlottedItem)
{
	// Remove the Item Description
	UInv_InventoryStatics::ItemUnhovered(GetOwningPlayer());

	if(IsValid(GetHoverItem()) && GetHoverItem()->IsStackable()) return;

	// Get Item to Equip
	UInv_InventoryItem* ItemToEquip = IsValid(GetHoverItem()) ? GetHoverItem()->GetInventoryItem() : nullptr;

	// Get Item to Unequip
	UInv_InventoryItem* ItemToUnequip = EquippedSlottedItem->GetInventoryItem();

	// Get the Equipped Grid Slot holding this item
	UInv_EquippedGridSlot* EquippedGridSlot = FindSlotWithEquippedItem(ItemToUnequip);

	// Clear the equipped grid slot of this item (set its inventory item to nullptr)
	ClearSlotOfItem(EquippedGridSlot);

	// Assign previously equipped item as the hover item
	Grid_Equippables->AssignHoverItem(ItemToUnequip);

	// Remove of the equipped slotted item from the equipped grid slot
	RemoveEquippedSlottedItem(EquippedSlottedItem);

	// Make a new equipped slotted item (for the item we held in HoverItem)
	MakeEquippedSlottedItem(EquippedSlottedItem, EquippedGridSlot, ItemToEquip);

	// Broadcast delegates for OnItemEquipped/OnItemUnequipped (from the IC)
	BroadcastSlotClickedDelegates(ItemToEquip, ItemToUnequip);
}

float UInv_SpatialInventory::GetTileSize() const
{
	return Grid_Equippables->GetTileSize();
}

UInv_EquippedGridSlot* UInv_SpatialInventory::FindSlotWithEquippedItem(UInv_InventoryItem* EquippedItem) const
{
	auto* FoundEquippedGridSlot = EquippedGridSlots.FindByPredicate([EquippedItem](const UInv_EquippedGridSlot* GridSlot)
	{
		return GridSlot->GetInventoryItem() == EquippedItem;
	});
	return FoundEquippedGridSlot ? *FoundEquippedGridSlot : nullptr;
}

void UInv_SpatialInventory::ClearSlotOfItem(UInv_EquippedGridSlot* EquippedGridSlot)
{
	if(IsValid(EquippedGridSlot))
	{
		EquippedGridSlot->SetEquippedSlottedItem(nullptr);
		EquippedGridSlot->SetInventoryItem(nullptr);
	}
}

void UInv_SpatialInventory::RemoveEquippedSlottedItem(UInv_EquippedSlottedItem* EquippedSlottedItem)
{
	if(!IsValid(EquippedSlottedItem)) return;

	if(EquippedSlottedItem->OnEquippedSlottedItemClicked.IsAlreadyBound(this, &ThisClass::EquippedSlottedItemClicked))
		EquippedSlottedItem->OnEquippedSlottedItemClicked.RemoveDynamic(this, &ThisClass::EquippedSlottedItemClicked);

	EquippedSlottedItem->RemoveFromParent();
}

void UInv_SpatialInventory::MakeEquippedSlottedItem(const UInv_EquippedSlottedItem* EquippedSlottedItem, UInv_EquippedGridSlot* EquippedGridSlot, UInv_InventoryItem* ItemToEquip)
{
	if(!IsValid(EquippedGridSlot)) return;

	UInv_EquippedSlottedItem* SlottedItem = EquippedGridSlot->OnItemEquipped(ItemToEquip, EquippedSlottedItem->GetEquipmentTypeTag(), UInv_InventoryStatics::GetInventoryWidget(GetOwningPlayer())->GetTileSize());
	if(IsValid(SlottedItem)) SlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this, &ThisClass::EquippedSlottedItemClicked);

	EquippedGridSlot->SetEquippedSlottedItem(SlottedItem);
}

void UInv_SpatialInventory::BroadcastSlotClickedDelegates(UInv_InventoryItem* ItemToEquip, UInv_InventoryItem* ItemToUnequip) const
{
	UInv_InventoryComponent* InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	check(IsValid(InventoryComponent));
	InventoryComponent->Server_EquipSlotClicked(ItemToEquip, ItemToUnequip);

	if(GetOwningPlayer()->GetNetMode() != NM_DedicatedServer)
	{
		InventoryComponent->OnItemEquipped.Broadcast(ItemToEquip);
		InventoryComponent->OnItemUnequipped.Broadcast(ItemToUnequip);
	}
}

void UInv_SpatialInventory::SetEquippedItemDescriptionSizeAndPosition(UInv_ItemDescription* Description, UInv_ItemDescription* EquippedDescription, UCanvasPanel* Canvas) const
{
	const UCanvasPanelSlot* ItemDescriptionCps = UWidgetLayoutLibrary::SlotAsCanvasSlot(Description);
	UCanvasPanelSlot* EquippedItemDescriptionCps = UWidgetLayoutLibrary::SlotAsCanvasSlot(EquippedDescription);
	if(!IsValid(ItemDescriptionCps) || !IsValid(EquippedItemDescriptionCps)) return;

	const FVector2D ItemDescriptionSize = Description->GetBoxSize();
	const FVector2D EquippedItemDescriptionSize = EquippedDescription->GetBoxSize();

	FVector2D ClampedPosition = UInv_WidgetUtils::GetClampedWidgetPosition(UInv_WidgetUtils::GetWidgetSize(Canvas), ItemDescriptionSize, UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer()));

	ClampedPosition.X -= EquippedItemDescriptionSize.X;

	EquippedItemDescriptionCps->SetSize(EquippedItemDescriptionSize);
	EquippedItemDescriptionCps->SetPosition(ClampedPosition);
}

void UInv_SpatialInventory::ShowEquippedItemDescription(UInv_InventoryItem* Item)
{
	const auto& Manifest = Item->GetItemManifest();
	const FInv_EquipmentFragment* EquipmentFragment = Manifest.GetFragmentOfType<FInv_EquipmentFragment>();
	if(!EquipmentFragment) return;

	const FGameplayTag HoveredEquipmentType = EquipmentFragment->GetEquipmentType();

	const auto EquippedGridSlot = EquippedGridSlots.FindByPredicate([Item](const UInv_EquippedGridSlot* GridSlot)
	{
		return GridSlot->GetInventoryItem() == Item;
	});
	if(EquippedGridSlot != nullptr) return; // The hovered item is already equipped, we're already showing its Item Description

	// It's not equipped, so find the equipped item with the same equipment type
	const auto FoundEquippedSlot = EquippedGridSlots.FindByPredicate([HoveredEquipmentType](const UInv_EquippedGridSlot* GridSlot)
	{
		const UInv_InventoryItem* InventoryItem = GridSlot->GetInventoryItem().Get();
		return IsValid(InventoryItem) ? InventoryItem->GetItemManifest().GetFragmentOfType<FInv_EquipmentFragment>()->GetEquipmentType() == HoveredEquipmentType : false;
	});
	const UInv_EquippedGridSlot* EquippedSlot = FoundEquippedSlot ? *FoundEquippedSlot : nullptr; // FoundEquippedSlot is a Double pointer (TObject pointer to a pointer)
	if(!IsValid(EquippedSlot)) return; // No equipped item with the same equipment type

	const UInv_InventoryItem* EquippedItem = EquippedSlot->GetInventoryItem().Get();
	if(!IsValid(EquippedItem)) return;

	const auto& EquippedItemManifest = EquippedItem->GetItemManifest();
	UInv_ItemDescription* DescriptionWidget = GetEquippedItemDescription();

	const auto EquippedDescriptionWidget = GetEquippedItemDescription();

	EquippedDescriptionWidget->Collapse();
	DescriptionWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	EquippedItemManifest.AssimilateInventoryFragments(EquippedDescriptionWidget);
}

UInv_ItemDescription* UInv_SpatialInventory::GetEquippedItemDescription()
{
	if(!IsValid(EquippedItemDescription) && IsValid(EquippedItemDescriptionClass))
	{
		EquippedItemDescription = CreateWidget<UInv_ItemDescription>(GetOwningPlayer(), EquippedItemDescriptionClass);
		CanvasPanel->AddChild(EquippedItemDescription);
	}
	return EquippedItemDescription;
}
