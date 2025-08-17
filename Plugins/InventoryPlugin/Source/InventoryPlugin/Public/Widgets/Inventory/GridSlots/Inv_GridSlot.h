// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlot.generated.h"

class UImage;
class UInv_InventoryItem;
class UInv_ItemPopUp;

UENUM(BlueprintType)
enum class EInv_GridSlotState : uint8
{
	Unoccupied,
	Occupied,
	Selected,
	GrayedOut
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGridSlotEvent, int32, GridIndex, const FPointerEvent&, MouseEvent);

/**
 * 
 */
UCLASS(PrioritizeCategories = ("Inventory"))
class INVENTORYPLUGIN_API UInv_GridSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetTileIndex(int32 Index) { TileIndex = Index; }

	int32 GetTileIndex() const { return TileIndex; }

	EInv_GridSlotState GetGridSlotState() const { return GridSlotState; }

	void SetOccupiedTexture();
	
	void SetUnoccupiedTexture();
	
	void SetSelectedTexture();
	
	void SetGrayedOutTexture();

	TWeakObjectPtr<UInv_InventoryItem> GetInventoryItem() const { return InventoryItem; }

	void SetInventoryItem(UInv_InventoryItem* Item);

	int32 GetStackCount() const { return StackCount; }

	void SetStackCount(int32 Count) { StackCount = Count; }

	int32 GetIndex() const { return TileIndex; }

	void SetIndex(int32 Index) { TileIndex = Index; }

	int32 GetUpperLeftIndex() const { return UpperLeftIndex; }

	void SetUpperLeftIndex(int32 Index) { UpperLeftIndex = Index; }

	bool IsAvailable() const { return bAvailable; }

	void SetAvailable(bool bIsAvailable) { bAvailable = bIsAvailable; }

	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& MouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	FGridSlotEvent GridSlotClicked;

	FGridSlotEvent GridSlotHovered;

	FGridSlotEvent GridSlotUnhovered;

	void SetItemPopUp(UInv_ItemPopUp* PopUp);

	UInv_ItemPopUp* GetItemPopUp() const;

private:
	int32 TileIndex{INDEX_NONE};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Unoccupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Occupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Selected;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_GrayedOut;

	EInv_GridSlotState GridSlotState;

	int32 StackCount{0};

	int32 UpperLeftIndex{INDEX_NONE};

	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;

	bool bAvailable{true};

	TWeakObjectPtr<UInv_ItemPopUp> ItemPopUp;

	UFUNCTION()
	void OnItemPopUpDestruct(UUserWidget* Menu);
};
