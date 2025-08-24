// 

#pragma once

#include "Modules/ModuleManager.h"

#define MessageLogListing "Inventory Plugin"
#define MessageLogLabel "Inventory Plugin Errors"

DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All);

class FInventoryPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
