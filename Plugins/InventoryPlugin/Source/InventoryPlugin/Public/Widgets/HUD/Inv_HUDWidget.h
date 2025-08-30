//

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_HUDWidget.generated.h"

class UInv_InfoMessage;

/**
 * Hud widget class for the inventory system.
 * Manages the display of inventory-related messages and information on the HUD.
 */
UCLASS(PrioritizeCategories = ("Inventory"))
class INVENTORYPLUGIN_API UInv_HUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void ShowPickupMessage(const FString& Message);

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void HidePickupMessage();
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InfoMessage> InfoMessage;

	UFUNCTION()
	void OnNoRoom();
};
