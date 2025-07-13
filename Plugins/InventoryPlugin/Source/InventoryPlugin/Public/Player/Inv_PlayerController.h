﻿// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inv_PlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
class UInv_HUDWidget;
class UInv_InventoryComponent;

/**
 * 
 */
UCLASS()
class INVENTORYPLUGIN_API AInv_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AInv_PlayerController();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void PrimaryInteract();

	void CreateHUDWidget();

	void TraceForItem();

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> PrimaryInteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_HUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UInv_HUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	double TraceLength;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

	TWeakObjectPtr<AActor> ThisActor;
	TWeakObjectPtr<AActor> LastActor;

	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> ToggleInventoryAction;
};
