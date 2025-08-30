//


#include "Widgets/ItemDescription/Inv_ItemDescription.h"
#include "Components/SizeBox.h"

FVector2D UInv_ItemDescription::GetBoxSize() const
{
	return SizeBox->GetDesiredSize();
}

void UInv_ItemDescription::SetVisibility(const ESlateVisibility InVisibility)
{
	for(const auto Child : GetChildren())
		Child->Collapse();

	Super::SetVisibility(InVisibility);
}
