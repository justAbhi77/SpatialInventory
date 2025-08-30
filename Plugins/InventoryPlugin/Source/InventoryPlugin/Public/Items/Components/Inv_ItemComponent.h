//

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Inv_ItemComponent.generated.h"


/**
 * Component that can be added to any Actor to make it an inventory item.
 * Contains an Item Manifest that defines the item's properties.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, PrioritizeCategories = ("Inventory"))
class INVENTORYPLUGIN_API UInv_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_ItemComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FString GetPickupMessage() const { return PickupMessage; }

	FInv_ItemManifest GetItemManifest() const { return ItemManifest; }

	void PickedUp();

	void InitItemManifest(const FInv_ItemManifest &CopyOfManifest);

	FInv_ItemManifest& GetItemManifestMutable() { return ItemManifest; }
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	// ReSharper disable once CppUEBlueprintImplementableEventNotImplemented
	void OnPickedUp();

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FString PickupMessage;

	UPROPERTY(Replicated, EditAnywhere, Category = "Inventory")
	FInv_ItemManifest ItemManifest;
};
