// 

#include "Items/Fragments/Inv_ItemFragment.h"
#include "Engine/Engine.h"

// OnConsume keep multiplayer in mind

void FInv_HealthPotionFragment::OnConsume(APlayerController* PlayerCont, int32 Count)
{
	// Get a stats component from the PlayerController or the PC->GetPawn()
	// or get the Ability System Component and apply a Gameplay Effect
	// or call an interface function for Healing()

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Health Potion consumed! Healing by: %f"), HealAmount * Count));
}

void FInv_ManaPotionFragment::OnConsume(APlayerController* PlayerCont, int32 Count)
{
	// Replenish mana however you wish

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Mana Potion consumed! Mana replenished by: %f"), ManaAmount * Count));
}
