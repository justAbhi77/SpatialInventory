﻿// 

#pragma once

#include "CoreMinimal.h"
#include "Inv_Highlightable.h"
#include "Components/StaticMeshComponent.h"
#include "Inv_HighlightableStaticMesh.generated.h"


/**
 * 
 */
UCLASS(PrioritizeCategories = ("Inventory"))
class INVENTORYPLUGIN_API UInv_HighlightableStaticMesh : public UStaticMeshComponent, public IInv_Highlightable
{
	GENERATED_BODY()
public:
	virtual void Highlight_Implementation() override;

	virtual void UnHighlight_Implementation() override;
private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UMaterialInterface> HighlightMaterial;
};
