// 

#include "Items/Manifest/Inv_ItemManifest.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Engine/World.h"
#include "InventoryPlugin.h"
#include "Logging/MessageLog.h"

UInv_InventoryItem* FInv_ItemManifest::Manifest(UObject* NewOuter)
{
	UInv_InventoryItem* Item = NewObject<UInv_InventoryItem>(NewOuter, UInv_InventoryItem::StaticClass());
	Item->SetItemManifest(*this);

	return Item;
}

void FInv_ItemManifest::SpawnPickupActor(const UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	if(!IsValid(PickupActorClass) || !IsValid(WorldContextObject)) return;

	AActor* SpawnedActor = WorldContextObject->GetWorld()->SpawnActor<AActor>(PickupActorClass, SpawnLocation, SpawnRotation);
	if(!IsValid(SpawnedActor)) return;

	// Set the item manifest, item category, item type, etc.
	UInv_ItemComponent* ItemComp = SpawnedActor->FindComponentByClass<UInv_ItemComponent>();

	if(!IsValid(ItemComp))
	{

#if WITH_EDITOR
		if(WorldContextObject->GetWorld()->GetNetMode() == NM_Client) return;
		auto messageLog = FMessageLog(MessageLogListing);
		messageLog.Error(FText::FromString(FString::Printf(TEXT("Item Component on Spawned Actor %s is not valid."), *SpawnedActor->GetName())));

		TSharedPtr<FDelegateHandle> ShutdownMessage = MakeShared<FDelegateHandle>();
		*ShutdownMessage = FEditorDelegates::ShutdownPIE.AddLambda([ShutdownMessage](bool bPlayInEditor){
			FMessageLog(MessageLogListing).Notify(FText::FromString("Inventory Item Spawn Error"), EMessageSeverity::Error, true);
			FEditorDelegates::ShutdownPIE.Remove(*ShutdownMessage);
		});
#endif // WITH_EDITOR

		UE_LOG(LogActor, BreakOnLog, TEXT("Item Component on Spawned Actor %s is not valid."), *SpawnedActor->GetName());
		return;
	}

	ItemComp->InitItemManifest(*this);
}
