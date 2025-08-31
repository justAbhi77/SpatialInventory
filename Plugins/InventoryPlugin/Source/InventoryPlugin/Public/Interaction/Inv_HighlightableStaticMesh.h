//

#pragma once

#include "CoreMinimal.h"
#include "Inv_Highlightable.h"
#include "Components/StaticMeshComponent.h"
#include "Inv_HighlightableStaticMesh.generated.h"

/**
 * Custom static mesh component that implements the IInv_Highlightable interface.
 */
UCLASS(PrioritizeCategories = ("Inventory"))
class INVENTORYPLUGIN_API UInv_HighlightableStaticMesh : public UStaticMeshComponent, public IInv_Highlightable
{
	GENERATED_BODY()
public:
	/**
	 * Implementation of the highlight logic for the mesh.
	 * Sets the overlay material to the HighlightMaterial.
	 */
	virtual void Highlight_Implementation() override;

	/**
	 * Implementation of the unhighlight logic for the mesh.
	 * Removes the overlay material.
	 */
	virtual void UnHighlight_Implementation() override;
private:
	// Material to use when highlighting the mesh.
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UMaterialInterface> HighlightMaterial;
};
