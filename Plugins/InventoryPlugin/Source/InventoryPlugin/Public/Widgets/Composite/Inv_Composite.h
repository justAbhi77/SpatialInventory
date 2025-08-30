//

#pragma once

#include "CoreMinimal.h"
#include "Inv_CompositeBase.h"
#include "Inv_Composite.generated.h"

/**
 * Middle level composite widget that can contain other composite widgets or leaf widgets.
 */
UCLASS(PrioritizeCategories = ("Inventory"))
class INVENTORYPLUGIN_API UInv_Composite : public UInv_CompositeBase
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	virtual void ApplyFunction(FUncType Function) override;

	virtual void Collapse() override;

	TArray<UInv_CompositeBase*> GetChildren() const { return Children; }
private:
	UPROPERTY()
	TArray<TObjectPtr<UInv_CompositeBase>> Children;
};
