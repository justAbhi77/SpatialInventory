//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inv_ProxyMesh.generated.h"

class UInv_EquipmentComponent;

/**
 * This class is used to display the character's mesh in the inventory menu.
 */
UCLASS(PrioritizeCategories = ("Inventory"))
class INVENTORYPLUGIN_API AInv_ProxyMesh : public AActor
{
	GENERATED_BODY()
public:
	AInv_ProxyMesh();

	USkeletalMeshComponent* GetMesh() const { return Mesh; }
protected:
	virtual void BeginPlay() override;
private:
	// This is the mesh on the player-controlled Character.
	TWeakObjectPtr<USkeletalMeshComponent> SourceMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInv_EquipmentComponent> EquipmentComponent;

	// This is the proxy mesh we will see in the Inventory Menu.
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	FTimerHandle TimerHandleForNextTick;

	UFUNCTION()
	void DelayedInitializeOwner();

	void DelayedInitialization();
};
