//

#include "InventoryPlugin.h"
#include "MessageLogInitializationOptions.h"
#include "MessageLogModule.h"

#define LOCTEXT_NAMESPACE "FInventoryPluginModule"

#define MESSAGE_LOG_MODULE "MessageLog"

DEFINE_LOG_CATEGORY(LogInventory);

void FInventoryPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Register the message log for this module
	FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>(MESSAGE_LOG_MODULE);
	FMessageLogInitializationOptions InitOptions;
	InitOptions.bShowPages = true;
	InitOptions.bAllowClear = true;
	InitOptions.bShowFilters = true;
	MessageLogModule.RegisterLogListing(MESSAGE_LOG_LISTING, NSLOCTEXT(MESSAGE_LOG_LISTING, MESSAGE_LOG_LABEL, MESSAGE_LOG_LISTING), InitOptions);
}

void FInventoryPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if(FModuleManager::Get().IsModuleLoaded("MessageLog"))
	{
		// unregister message log
		FMessageLogModule& MessageLogModule = FModuleManager::GetModuleChecked<FMessageLogModule>(MESSAGE_LOG_MODULE);
		MessageLogModule.UnregisterLogListing(MESSAGE_LOG_LISTING);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FInventoryPluginModule, InventoryPlugin)