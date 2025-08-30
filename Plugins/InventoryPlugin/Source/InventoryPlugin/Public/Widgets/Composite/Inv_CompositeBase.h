//

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "Inv_CompositeBase.generated.h"

/**
 * Base Widget class for Composite Pattern
 */
UCLASS(PrioritizeCategories = ("Inventory"))
class INVENTORYPLUGIN_API UInv_CompositeBase : public UUserWidget
{
	GENERATED_BODY()
public:
	FGameplayTag GetFragmentTag() const { return FragmentTag; }

	void SetFragmentTag(const FGameplayTag& Tag) { FragmentTag = Tag; }

	virtual void Collapse();

	void Expand();

	// using keyword is like typedef but more modern
	using FUncType = TFunction<void(UInv_CompositeBase*)>;
	virtual void ApplyFunction(FUncType Function) {}
private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag FragmentTag;
};
