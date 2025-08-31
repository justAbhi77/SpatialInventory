//

#include "Player/Inv_PlayerController.h"
#include "InventoryPlugin.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Widgets/HUD/Inv_HUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Interaction/Inv_Highlightable.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"

AInv_PlayerController::AInv_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set default trace length and channel for item interaction
	TraceLength = 500.0;
	ItemTraceChannel = ITEM_TRACE_CHANNEL;
}

void AInv_PlayerController::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check for interactable items each frame
	TraceForItem();
}

void AInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Set up input mapping contexts for this player
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		for(const UInputMappingContext* CurrentContext : DefaultImc)
			Subsystem->AddMappingContext(CurrentContext, 0);

	// Find the inventory component on this player
	InventoryComponent = FindComponentByClass<UInv_InventoryComponent>();

	CreateHudWidget();
}

void AInv_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComp = CastChecked<UEnhancedInputComponent>(InputComponent);

	// Bind primary interact and inventory toggle actions
	EnhancedInputComp->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &AInv_PlayerController::PrimaryInteract);
	EnhancedInputComp->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AInv_PlayerController::ToggleInventory);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AInv_PlayerController::PrimaryInteract()
{
	if(!CurrentTracedActor.IsValid()) return;

	UInv_ItemComponent* ItemComp = CurrentTracedActor->FindComponentByClass<UInv_ItemComponent>();
	if(!IsValid(ItemComp) || !InventoryComponent.IsValid()) return;

	// Attempt to add the item to the player's inventory
	// There may not be enough space in the inventory for all the item's stack count
	InventoryComponent->TryAddItem(ItemComp);
}

void AInv_PlayerController::CreateHudWidget()
{
	if(!IsLocalController()) return;

	HudWidget = CreateWidget<UInv_HUDWidget>(this, HudWidgetClass);
	if(IsValid(HudWidget))
		HudWidget->AddToViewport();
}

void AInv_PlayerController::TraceForItem()
{
	if(!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;

	// Get the center of the screen for tracing
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f;

	FVector TraceStart, Forward;
	// Convert screen position to world direction
	if(!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter, TraceStart, Forward)) return;

	const FVector TraceEnd = TraceStart + Forward * TraceLength;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ItemTraceChannel);

	LastTracedActor = CurrentTracedActor;
	CurrentTracedActor = HitResult.GetActor();

	// If nothing was hit this frame
	if(!CurrentTracedActor.IsValid())
		if(IsValid(HudWidget)) HudWidget->HidePickupMessage();

	// If target actor changed, update highlights
	if(CurrentTracedActor != LastTracedActor)
	{
		// Highlight the new actor, if it supports highlighting
		if(CurrentTracedActor.IsValid())
		{
			if(UActorComponent* Highlightable = CurrentTracedActor->FindComponentByInterface(UInv_Highlightable::StaticClass()))
				IInv_Highlightable::Execute_Highlight(Highlightable);

			if(const UInv_ItemComponent* ItemComponent = CurrentTracedActor->FindComponentByClass<UInv_ItemComponent>(); IsValid(ItemComponent) && IsValid(HudWidget))
				HudWidget->ShowPickupMessage(ItemComponent->GetPickupMessage());
		}

		// Unhighlight the previous actor, if it supported highlighting
		if(LastTracedActor.IsValid())
		{
			if(UActorComponent* Highlightable = LastTracedActor->FindComponentByInterface(UInv_Highlightable::StaticClass()))
				IInv_Highlightable::Execute_UnHighlight(Highlightable);
		}
	}
}

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
void AInv_PlayerController::ToggleInventory()
{
	if(!InventoryComponent.IsValid()) return;

	// Toggle the inventory menu and handle HUD visibility accordingly
	InventoryComponent->ToggleInventoryMenu();

	if(InventoryComponent->IsMenuOpen() && IsValid(HudWidget))
		HudWidget->SetVisibility(ESlateVisibility::Hidden);
	else if(IsValid(HudWidget))
		HudWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}
