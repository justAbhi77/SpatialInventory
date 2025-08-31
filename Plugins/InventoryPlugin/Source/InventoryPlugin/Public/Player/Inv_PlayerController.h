//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inv_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UInv_HUDWidget;
class UInv_InventoryComponent;

/**
 * Player controller for inventory plugin.
 * Handles input, item tracing, inventory toggling, and HUD management.
 */
UCLASS(PrioritizeCategories = ("Inventory"))
class INVENTORYPLUGIN_API AInv_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AInv_PlayerController();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
private:
	// Handles when the interact key is pressed via Enhanced Input system.
	void PrimaryInteract();

	// Creates and adds the inventory HUD widget to the viewport.
	void CreateHudWidget();

	// Traces to detect highlight/interact actors in world.
	void TraceForItem();

	// Input mapping contexts to set up default controls.
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TArray<TObjectPtr<UInputMappingContext>> DefaultImc;

	// Input Action for primary interaction (e.g., Highlighting and picking up items).
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> PrimaryInteractAction;

	// The widget class for the Hud.
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_HUDWidget> HudWidgetClass;

	// Reference to the created Hud widget.
	UPROPERTY()
	TObjectPtr<UInv_HUDWidget> HudWidget;

	// Maximum length for item line traces (how far player can interact).
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	double TraceLength;

	// Trace channel used for detecting inventory items.
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

	// Current and Last actor that was traced (to handle highlight transitions).
	TWeakObjectPtr<AActor> CurrentTracedActor, LastTracedActor;

	// Reference to the player's inventory component.
	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;

	// Input Action for toggling the inventory UI.
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> ToggleInventoryAction;
};
