//

#include "Widgets/HUD/Inv_HUDWidget.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Widgets/HUD/Inv_InfoMessage.h"

void UInv_HUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(UInv_InventoryComponent* InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer()); IsValid(InventoryComponent))
		InventoryComponent->NoRoomInInventory.AddDynamic(this, &UInv_HUDWidget::OnNoRoom);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UInv_HUDWidget::OnNoRoom()
{
	if(!IsValid(InfoMessage)) return;
	InfoMessage->SetMessage(FText::FromString("No Room In Inventory."));
}
