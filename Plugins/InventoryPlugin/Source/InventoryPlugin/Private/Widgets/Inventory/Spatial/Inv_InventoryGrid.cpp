// 

#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"


void UInv_InventoryGrid::NativePreConstruct()
{
	Super::NativePreConstruct();

	ConstructGrid();
}

void UInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// we do not construct the grid here 
	// rather the Spatial Inventory will call ConstructGrid after forcing the layout prepass so that the grid is properly sized

	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::AddItem);
}

void UInv_InventoryGrid::ConstructGrid()
{
	GridSlots.Empty();
	CanvasPanel->ClearChildren();
	GridSlots.Reserve(Rows * Columns);

	if(!GridSlotClass) return;

	// row major order for creating and storing tiles in an array.
	// this allows us to easily access the tiles by index later
	for(int32 j = 0; j < Rows; ++j)
	{
		for(int32 i = 0; i < Columns; ++i)
		{
			UInv_GridSlot* GridSlot = CreateWidget<UInv_GridSlot>(this, GridSlotClass);
			CanvasPanel->AddChild(GridSlot);

			const FIntPoint TilePosition(i, j);
			GridSlot->SetTileIndex(UInv_WidgetUtils::GetIndexFromPosition(TilePosition, Columns));

			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCPS->SetSize(FVector2D(TileSize));
			GridCPS->SetPosition(TilePosition * TileSize);

			GridSlots.Add(GridSlot);
		}
	}
}

void UInv_InventoryGrid::CalculateTileSize(FVector2D DesiredSize)
{
	if(Rows <= 0 || Columns <= 0)
	{
		TileSize = 75.f;
		return;
	}

	if(DesiredSize.IsZero())
	{
		TileSize = 75.f; // default tile size
		return;
	}

	const float Width = DesiredSize.X / Columns;
	const float Height = DesiredSize.Y / Rows;

	TileSize = FMath::Min(Width, Height);
}

void UInv_InventoryGrid::AddItem(UInv_InventoryItem* Item)
{
	if(!MatchesCategory(Item)) return;

	UE_LOG(LogTemp, Warning, TEXT("InventoryGrid::AddItem"));
}

bool UInv_InventoryGrid::MatchesCategory(const UInv_InventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}
