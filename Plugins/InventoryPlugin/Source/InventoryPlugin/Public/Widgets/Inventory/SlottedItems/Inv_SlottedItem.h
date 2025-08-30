//

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_SlottedItem.generated.h"

class UInv_InventoryItem;
class UImage;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSlottedItemClicked, int32, GridIndex, const FPointerEvent&, MouseEvent);

/**
* Inventory item widget that can be placed in a grid slot
*/
UCLASS(PrioritizeCategories = ("Inventory"))
class INVENTORYPLUGIN_API UInv_SlottedItem : public UUserWidget
{
	GENERATED_BODY()
public:
	bool IsStackable() const { return bIsStackable; }

	void SetIsStackable(const bool bStackable) { bIsStackable = bStackable; }

	UImage* GetImageIcon() const { return Image_Icon; }

	void SetGridIndex(const int32 Index) { GridIndex = Index; }

	int32 GetGridIndex() const { return GridIndex; }

	void SetGridDimensions(const FIntPoint& Dimensions) { GridDimensions = Dimensions; }

	FIntPoint GetGridDimensions() const { return GridDimensions; }

	void SetInventoryItem(UInv_InventoryItem* Item);

	UInv_InventoryItem* GetInventoryItem() const { return InventoryItem.Get(); }

	void SetImageBrush(const FSlateBrush& Brush) const;

	void UpdateStackCount(int32 StackCount) const;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	FSlottedItemClicked OnSlottedItemClicked;

	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& MouseEvent) override;
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	int32 GridIndex;

	FIntPoint GridDimensions;

	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;

	bool bIsStackable{false};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StackCount;
};
