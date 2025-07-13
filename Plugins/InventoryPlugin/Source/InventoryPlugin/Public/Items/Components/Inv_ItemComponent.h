// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inv_ItemComponent.generated.h"


/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORYPLUGIN_API UInv_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_ItemComponent();

	FString GetPickupMessage() const { return PickupMessage; }
protected:


private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FString PickupMessage;
};
