//

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_CharacterDisplay.generated.h"

/**
 * Widget to display a character mesh in UI that can be rotated by dragging the mouse.
 */
UCLASS(PrioritizeCategories = ("Inventory"))
class INVENTORYPLUGIN_API UInv_CharacterDisplay : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
protected:
	UFUNCTION(BlueprintCallable)
	void OnMouseUp();
private:
	bool bIsDragging{false};
	TWeakObjectPtr<USkeletalMeshComponent> Mesh;

	FVector2D CurrentPosition, LastPosition;

	UPROPERTY(EditAnywhere)
	bool bShouldStopWhenMouseLeaves{false};
};
