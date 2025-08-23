// 

#include "Widgets/Inventory/SlottedItems/Inv_EquippedSlottedItem.h"

FReply UInv_EquippedSlottedItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnEquippedSlottedItemClicked.Broadcast(this);
	return FReply::Handled();
}
