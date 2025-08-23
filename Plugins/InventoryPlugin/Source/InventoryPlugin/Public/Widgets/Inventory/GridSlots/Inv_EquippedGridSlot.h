﻿// 

#pragma once

#include "CoreMinimal.h"
#include "Inv_GridSlot.h"
#include "GameplayTagContainer.h"
#include "Inv_EquippedGridSlot.generated.h"

class UImage;
class UInv_EquippedSlottedItem;
class UOverlay;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquippedGridSlotClicked, UInv_EquippedGridSlot*, GridSlot, const FGameplayTag&, EquipmentTypeTag);

/**
* 
*/
UCLASS(PrioritizeCategories = ("Inventory"))
class INVENTORYPLUGIN_API UInv_EquippedGridSlot : public UInv_GridSlot
{
	GENERATED_BODY()
public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UInv_EquippedSlottedItem* OnItemEquipped(UInv_InventoryItem* Item, const FGameplayTag& EquipmentTag, float TileSize);

	FEquippedGridSlotClicked EquippedGridSlotClicked;

	void SetEquippedSlottedItem(UInv_EquippedSlottedItem* Item) { EquippedSlottedItem = Item; }
private:
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories = "GameItems.Equipment"))
	FGameplayTag EquipmentTypeTag;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GrayedOutIcon;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_EquippedSlottedItem> EquippedSlottedItemClass;

	UPROPERTY()
	TObjectPtr<UInv_EquippedSlottedItem> EquippedSlottedItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_Root;
};
