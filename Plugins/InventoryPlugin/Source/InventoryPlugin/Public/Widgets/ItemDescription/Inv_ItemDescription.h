//

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Composite/Inv_Composite.h"
#include "Inv_ItemDescription.generated.h"

class USizeBox;

/**
 * Simple wrapper for a size box to hold item descriptions.
 * This allows us to easily get the size of the description box.
 * Managed through the composite pattern by the inventory item.
 */
UCLASS(PrioritizeCategories = ("Inventory"))
class INVENTORYPLUGIN_API UInv_ItemDescription : public UInv_Composite
{
	GENERATED_BODY()
public:
	FVector2D GetBoxSize() const;

	virtual void SetVisibility(ESlateVisibility InVisibility) override;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;
};
