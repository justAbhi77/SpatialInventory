//

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inv_Highlightable.generated.h"

/**
 * Interface for actor components that can be highlighted or unhighlighted by the inventory system.
 * Any component that implements this interface can be interacted with visually (highlight/unhighlight)
 * when the player looks at it.
 */
UINTERFACE()
class UInv_Highlightable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for actor components that can be highlighted or unhighlighted by the inventory system.
 * Any component that implements this interface can be interacted with visually (highlight/unhighlight)
 * when the player looks at it.
 */
class INVENTORYPLUGIN_API IInv_Highlightable
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory")
	void Highlight();

	UFUNCTION(BlueprintNativeEvent, Category = "Inventory")
	void UnHighlight();
};
