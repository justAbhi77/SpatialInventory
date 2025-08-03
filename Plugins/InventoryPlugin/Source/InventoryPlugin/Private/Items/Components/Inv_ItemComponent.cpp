// 

#include "Items/Components/Inv_ItemComponent.h"
#include "Delegates/IDelegateInstance.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Logging/MessageLog.h"
#include "Editor.h"
#include "InventoryPlugin.h"


UInv_ItemComponent::UInv_ItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PickupMessage = FString("E - Pick Up");
}

void UInv_ItemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
}

void UInv_ItemComponent::BeginPlay() 
{
	Super::BeginPlay();

	if(AActor* Owner = GetOwner())
		if(!Owner->GetIsReplicated())
		{
			// UE_LOG(LogActor, BreakOnLog, TEXT("Item Component on Actor %s is Not Replicated."), *Owner->GetName());

			auto messageLog = FMessageLog(MessageLogListing);
			messageLog.Error(FText::FromString(FString::Printf(TEXT("Item Component on Actor %s is Not Replicated."), *Owner->GetName())));

#if WITH_EDITOR

			TSharedPtr<FDelegateHandle> ShutdownMessage = MakeShared<FDelegateHandle>();
			*ShutdownMessage = FEditorDelegates::ShutdownPIE.AddLambda([ShutdownMessage](bool bPlayInEditor){
				FMessageLog(MessageLogListing).Notify(FText::FromString("Inventory Item Actor Error"), EMessageSeverity::Error, true);
				FEditorDelegates::ShutdownPIE.Remove(*ShutdownMessage);
			});
#endif // WITH_EDITOR

		}
}

void UInv_ItemComponent::PickedUp()
{
	OnPickedUp();
	GetOwner()->Destroy();
}
