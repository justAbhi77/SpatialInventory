// 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Composite/Inv_Composite.h"
#include "Inv_ItemDescription.generated.h"

class USizeBox;

/**
 * 
 */
UCLASS(PrioritizeCategories = ("Inventory"))
class INVENTORYPLUGIN_API UInv_ItemDescription : public UInv_Composite
{
	GENERATED_BODY()
public:
	FVector2D GetBoxSize() const;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;
};
