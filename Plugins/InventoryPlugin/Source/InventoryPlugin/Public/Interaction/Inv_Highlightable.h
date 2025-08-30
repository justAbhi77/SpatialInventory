//

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inv_Highlightable.generated.h"


// This class does not need to be modified.
UINTERFACE()
class UInv_Highlightable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Any actor that implements this interface can be highlighted/unhighlighted by the inventory system when the player looks at it.
 */
class INVENTORYPLUGIN_API IInv_Highlightable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, Category = "Inventory")
	void Highlight();

	UFUNCTION(BlueprintNativeEvent, Category = "Inventory")
	void UnHighlight();
};
