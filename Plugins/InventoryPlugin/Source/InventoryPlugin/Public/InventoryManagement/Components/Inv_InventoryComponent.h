//

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryManagement/FastArray/Inv_FastArray.h"
#include "Inv_InventoryComponent.generated.h"

class UInv_InventoryBase;
class UInv_InventoryItem;
class UInv_ItemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, UInv_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackChange, const FInv_SlotAvailabilityResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemEquipStatusChanged, UInv_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryMenuToggled, bool, bOpen);

/**
 * Inventory component to add inventory functionality to any Player Controller, such as a player character.
 * Handles adding/removing items, item stacking, menu management, input, networking, and updates.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable, PrioritizeCategories = ("Inventory"))
class INVENTORYPLUGIN_API UInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_InventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Toggles the inventory menu (open/close) and updates Hud as needed.
	void ToggleInventoryMenu();

	// Delegates for inventory item change, equip/unequip, etc.
	FInventoryItemChange OnItemAdded;
	FInventoryItemChange OnItemRemoved;
	FNoRoomInInventory NoRoomInInventory;
	FStackChange OnStackChange;

	// Attempt to add an item to the inventory through C++ or Blueprint.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void TryAddItem(UInv_ItemComponent* ItemComponent);

	// Server RPC: add new item to inventory.
	UFUNCTION(Server, Reliable)
	void Server_AddNewItem(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	// Server RPC: add stacks to an existing inventory item.
	UFUNCTION(Server, Reliable)
	void Server_AddStacksToItem(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	// Register a replicated subobject for Unreal's fast array replication.
	void AddRepSubObj(UObject* SubObj);

	// Server RPC: drop an item from the inventory, removing or reducing its stack.
	UFUNCTION(Server, Reliable)
	void Server_DropItem(UInv_InventoryItem* Item, int32 StackCount);

	// Utility: spawns the dropped item actor in the world at a calculated location.
	void SpawnDroppedItem(UInv_InventoryItem* Item, int32 StackCount) const;

	// Server RPC: consume part of an item's stack, removing it if stack reaches 0.
	UFUNCTION(Server, Reliable)
	void Server_ConsumeItem(UInv_InventoryItem* Item, int32 StackCount);

	// Returns the inventory menu widget, if any.
	UInv_InventoryBase* GetInventoryMenu() const { return InventoryMenu; }

	// Server RPC: called when attempting to equip/unequip an item slot from the UI.
	UFUNCTION(Server, Reliable)
	void Server_EquipSlotClicked(UInv_InventoryItem* ItemToEquip, UInv_InventoryItem* ItemToUnequip);

	// Multicast RPC: sync slot equip/unequip across networked clients.
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EquipSlotClicked(UInv_InventoryItem* ItemToEquip, UInv_InventoryItem* ItemToUnequip);

	// Events for binding when item equipped/unequipped, inventory menu toggled.
	FItemEquipStatusChanged OnItemEquipped;
	FItemEquipStatusChanged OnItemUnequipped;
	FInventoryMenuToggled OnInventoryMenuToggled;

	// Query if the inventory menu is currently open.
	bool IsMenuOpen() const { return bInventoryMenuOpen; }
protected:
	virtual void BeginPlay() override;
private:
	// Weak pointer to the owning player controller.
	TWeakObjectPtr<APlayerController> OwningController;

	// Handle the inventory menu widget construction.
	void ConstructInventory();

	// Reference to the inventory menu UI widget.
	UPROPERTY()
	TObjectPtr<UInv_InventoryBase> InventoryMenu;

	// Inventory menu UI class to instantiate.
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_InventoryBase> InventoryMenuClass;

	bool bInventoryMenuOpen;

	// Open the inventory menu, set input, and show mouse.
	void OpenInventoryMenu();

	// Close the inventory menu, set input, and hide mouse.
	void CloseInventoryMenu();

	// Replicated fast array for inventory contents.
	UPROPERTY(Replicated)
	FInv_InventoryFastArray InventoryList;

	// Properties for random drop angle/distance when spawning dropped items
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnAngleMin = -90.f;

	// Properties for random drop angle/distance when spawning dropped items
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnAngleMax = 90.f;

	// Properties for random drop angle/distance when spawning dropped items
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnDistanceMin = 50.f;

	// Properties for random drop angle/distance when spawning dropped items
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnDistanceMax = 125.f;

	// Properties for random drop angle/distance when spawning dropped items
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float RelativeSpawnDepression = 75.f;
};
