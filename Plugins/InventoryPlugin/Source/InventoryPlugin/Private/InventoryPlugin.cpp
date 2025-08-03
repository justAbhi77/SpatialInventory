// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryPlugin.h"
#include "MessageLogInitializationOptions.h"
#include "MessageLogModule.h"


#define LOCTEXT_NAMESPACE "FInventoryPluginModule"

DEFINE_LOG_CATEGORY(LogInventory);

void FInventoryPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Register the message log for this module
	FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
	FMessageLogInitializationOptions InitOptions;
	InitOptions.bShowPages = true;
	InitOptions.bAllowClear = true;
	InitOptions.bShowFilters = true;
	MessageLogModule.RegisterLogListing(MessageLogListing, NSLOCTEXT(MessageLogListing, MessageLogLabel, MessageLogListing), InitOptions);
}

void FInventoryPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if(FModuleManager::Get().IsModuleLoaded("MessageLog"))
	{
		// unregister message log
		FMessageLogModule& MessageLogModule = FModuleManager::GetModuleChecked<FMessageLogModule>("MessageLog");
		MessageLogModule.UnregisterLogListing(MessageLogListing);
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInventoryPluginModule, InventoryPlugin)