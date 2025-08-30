//


#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Types/Inv_GridTypes.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"

UInv_InventoryComponent* UInv_InventoryStatics::GetInventoryComponent(const APlayerController* PlayerController)
{
	if(!IsValid(PlayerController)) return nullptr;

	UInv_InventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<UInv_InventoryComponent>();
	return InventoryComponent;
}

EInv_ItemCategory UInv_InventoryStatics::GetItemCategoryFromItemComp(UInv_ItemComponent* ItemComp)
{
	if(!IsValid(ItemComp)) return EInv_ItemCategory::None;
	return ItemComp->GetItemManifest().GetItemCategory();
}

void UInv_InventoryStatics::ItemHovered(APlayerController* PC, UInv_InventoryItem* Item)
{
	const UInv_InventoryComponent* Ic = GetInventoryComponent(PC);
	if(!IsValid(Ic)) return;

	UInv_InventoryBase* InventoryBase = Ic->GetInventoryMenu();
	if(!IsValid(InventoryBase)) return;

	if(InventoryBase->HasHoverItem()) return;

	InventoryBase->OnItemHovered(Item);
}

void UInv_InventoryStatics::ItemUnhovered(APlayerController* PC)
{
	const UInv_InventoryComponent* Ic = GetInventoryComponent(PC);
	if(!IsValid(Ic)) return;

	UInv_InventoryBase* InventoryBase = Ic->GetInventoryMenu();
	if(!IsValid(InventoryBase)) return;

	InventoryBase->OnItemUnHovered();
}

UInv_HoverItem* UInv_InventoryStatics::GetHoverItem(APlayerController* PC)
{
	const UInv_InventoryComponent* Ic = GetInventoryComponent(PC);
	if(!IsValid(Ic)) return nullptr;

	const UInv_InventoryBase* InventoryBase = Ic->GetInventoryMenu();
	if(!IsValid(InventoryBase)) return nullptr;

	return InventoryBase->GetHoverItem();
}

UInv_InventoryBase* UInv_InventoryStatics::GetInventoryWidget(const APlayerController* PC)
{
	const UInv_InventoryComponent* Ic = GetInventoryComponent(PC);
	if(!IsValid(Ic)) return nullptr;

	return Ic->GetInventoryMenu();
}
