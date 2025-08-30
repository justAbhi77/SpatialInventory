//


#include "Widgets/Composite/Inv_Leaf.h"

void UInv_Leaf::ApplyFunction(const FUncType Function)
{
	Function(this);
}
