//

#pragma once

#include "CoreMinimal.h"
#include "Inv_CompositeBase.h"
#include "Inv_Leaf.generated.h"

/**
 * Peripheral Widget base class for Composite Pattern
 */
UCLASS(PrioritizeCategories = ("Inventory"))
class INVENTORYPLUGIN_API UInv_Leaf : public UInv_CompositeBase
{
	GENERATED_BODY()
public:
	virtual void ApplyFunction(FUncType Function) override;
};
