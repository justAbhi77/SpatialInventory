// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"


class UCanvasPanel;
class UInv_GridSlot;
class UInv_InventoryComponent;

/**
 * 
 */
UCLASS()
class INVENTORYPLUGIN_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;

	virtual void NativeOnInitialized() override;

	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ConstructGrid();
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CalculateTileSize(FVector2D DesiredSize);

	UFUNCTION()
	void AddItem(UInv_InventoryItem* Item);
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
};
