﻿// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"


class UCanvasPanel;
class UInv_GridSlot;
class UInv_InventoryComponent;
class UInv_ItemComponent;
struct FInv_ItemManifest;
struct FInv_ImageFragment;
struct FInv_GridFragment;
class UInv_SlottedItem;
struct FGameplayTag;
class UInv_HoverItem;
enum class EInv_GridSlotState : uint8;
class UInv_ItemPopUp;

/**
 * 
 */
UCLASS(PrioritizeCategories = ("Inventory"))
class INVENTORYPLUGIN_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;

	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ConstructGrid();
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CalculateTileSize(FVector2D DesiredSize);

	UFUNCTION()
	void AddItem(UInv_InventoryItem* Item);

	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_ItemComponent* ItemComponent);

	void ShowCursor();

	void HideCursor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool bCheckForSlottedItemsWithRoom{true};

	void SetOwningCanvas(UCanvasPanel* OwningCanvas);

	void DropItem();

	bool HasHoverItem() const;

	UInv_HoverItem* GetHoverItem() const;

	float GetTileSize() const { return TileSize; }

	void ClearHoverItem();

	void AssignHoverItem(UInv_InventoryItem* InventoryItem);

	void OnHide();
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),  Category = "Inventory")
	EInv_ItemCategory ItemCategory;

	UPROPERTY()
	TArray<TObjectPtr<UInv_GridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_GridSlot> GridSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Inventory")
	int32 Rows;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Inventory")
	int32 Columns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Inventory")
	float TileSize;

	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;

	bool MatchesCategory(const UInv_InventoryItem* Item) const;

	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_InventoryItem* Item, const int32 StackAmountOverride = -1);
	FInv_SlotAvailabilityResult HasRoomForItem(const FInv_ItemManifest& Manifest, const int32 StackAmountOverride = -1);

	void AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* NewItem);

	FVector2D GetDrawSize(const FInv_GridFragment* GridFragment) const;

	void SetSlottedItemImage(const UInv_SlottedItem* SlottedItem, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment) const;

	void AddItemAtIndex(UInv_InventoryItem* Item, const int32 Index, const bool bStackable, const int32 StackAmount);

	UInv_SlottedItem* CreateSlottedItem(UInv_InventoryItem* Item, const bool bStackable, const int32 StackAmount, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment, const int32 Index);

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_SlottedItem> SlottedItemClass;

	void AddSlottedItemToCanvas(const int32 Index, const FInv_GridFragment* GridFragment, UInv_SlottedItem* SlottedItem) const;

	void UpdateGridSlots(UInv_InventoryItem* NewItem, const int32 Index,  bool bStackableItem, const int32 StackAmount);

	UPROPERTY()
	TMap<int32, TObjectPtr<UInv_SlottedItem>> SlottedItems;

	bool IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const;

	bool HasRoomAtIndex(const UInv_GridSlot* GridSlot, const FIntPoint& Dimensions, const TSet<int32>& CheckedIndices, TSet<int32>& OutTentativelyClaimed, const FGameplayTag& ItemType, const int32 MaxStackSize);

	FIntPoint GetItemDimensions(const FInv_ItemManifest& Manifest) const;

	bool CheckSlotConstraints(const UInv_GridSlot* GridSlot, const UInv_GridSlot* SubGridSlot, const TSet<int32>& CheckedIndices, TSet<int32>& OutTentativelyClaimed, const FGameplayTag& ItemType, const int32 MaxStackSize) const;

	bool HasValidItem(const UInv_GridSlot* GridSlot) const;

	bool IsUpperLeftSlot(const UInv_GridSlot* GridSlot, const UInv_GridSlot* SubGridSlot) const;

	bool DoesItemTypeMatch(const UInv_InventoryItem* SubItem, const FGameplayTag& ItemType) const;

	bool IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const;

	int32 DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize, const int32 AmountToFill, const UInv_GridSlot* GridSlot) const;

	int32 GetStackAmount(const UInv_GridSlot* GridSlot) const;

	UFUNCTION()
	void AddStacks(const FInv_SlotAvailabilityResult& Result);

	UFUNCTION()
	void OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	bool IsRightClick(const FPointerEvent& MouseEvent) const;

	bool IsLeftClick(const FPointerEvent& MouseEvent) const;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_HoverItem> HoverItemClass;

	UPROPERTY()
	TObjectPtr<UInv_HoverItem> HoverItem;

	void PickUp(UInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex);

	void AssignHoverItem(UInv_InventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviousGridIndex);

	void RemoveItemFromGrid(UInv_InventoryItem* InventoryItem, const int32 GridIndex);

	FInv_TileParameters TileParameters;

	FInv_TileParameters LastTileParameters;

	void UpdateTileParameters(const FVector2D& CanvasPosition, const FVector2D& MousePosition);

	FIntPoint CalculateHoveredCoordinates(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;

	EInv_TileQuadrant CalculateTileQuadrant(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;

	void OnTileParametersUpdated(const FInv_TileParameters& Parameters);

	FIntPoint CalculateStartingCoordinate(const FIntPoint& Coordinate, const FIntPoint& Dimensions, const EInv_TileQuadrant Quadrant) const;

	FInv_SpaceQueryResult CheckHoverPosition(const FIntPoint& Position, const FIntPoint& Dimensions);

	// Index where an item would be placed if we click on the grid at a valid location
	int32 ItemDropIndex{INDEX_NONE};
	FInv_SpaceQueryResult CurrentQueryResult;

	bool CursorExitedCanvas(const FVector2D& BoundaryPos, const FVector2D& BoundarySize, const FVector2D& Location);

	bool bMouseWithinCanvas, bLastMouseWithinCanvas;

	void HighlightSlots(const int32 Index, const FIntPoint& Dimensions);

	void UnHighlightSlots(const int32 Index, const FIntPoint& Dimensions);

	int32 LastHighlightedIndex;

	FIntPoint LastHighlightedDimensions;

	void ChangeHoverType(const int32 Index, const FIntPoint& Dimensions, EInv_GridSlotState GridSlotState);

	UFUNCTION()
	void OnGridSlotClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnGridSlotHovered(int32 GridIndex, const FPointerEvent& MouseEvent);

	UFUNCTION()
	void OnGridSlotUnhovered(int32 GridIndex, const FPointerEvent& MouseEvent);

	void PutDownOnIndex(const int32 Index);

	UUserWidget* GetVisibleCursorWidget();

	UUserWidget* GetHiddenCursorWidget();

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UUserWidget> VisibleCursorWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UUserWidget> HiddenCursorWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> VisibleCursorWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> HiddenCursorWidget;

	bool IsSameStackable(const UInv_InventoryItem* ClickedInventoryItem) const;

	void SwapWithHoverItem(UInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex);

	bool ShouldSwapStackCounts(const int32 RoomInClickedSlot, const int32 HoveredStackCount, const int32 MaxStackSize) const;

	void SwapStackCounts(const int32 ClickedStackCount, const int32 HoveredStackCount, const int32 Index);

	bool ShouldConsumeHoverItemStacks(const int32 HoveredStackCount, const int32 RoomInClickedSlot) const;

	void ConsumeHoverItemStacks(const int32 ClickedStackCount, const int32 HoveredStackCount, const int32 Index);

	bool ShouldFillInStack(const int32 RoomInClickedSlot, const int32 HoveredStackCount) const;

	void FillInStack(const int32 FillAmount, const int32 Remainder, const int32 Index);

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_ItemPopUp> ItemPopUpClass;

	UPROPERTY()
	TObjectPtr<UInv_ItemPopUp> ItemPopUp;

	TWeakObjectPtr<UCanvasPanel> OwningCanvasPanel;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FVector2D ItemPopUpOffset;

	UFUNCTION()
	void OnPopUpMenuSplit(int32 SplitAmount, int32 Index);

	UFUNCTION()
	void OnPopUpMenuDrop(int32 Index);

	UFUNCTION()
	void OnPopUpMenuConsume(int32 Index);

	void CreateItemPopUp(const int32 GridIndex);

	void PutHoverItemBack();

	UFUNCTION()
	void OnInventoryMenuToggled(bool bOpen);
};
