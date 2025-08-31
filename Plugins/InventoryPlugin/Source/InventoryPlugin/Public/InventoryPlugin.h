//

#pragma once

#include "Modules/ModuleManager.h"

#define MESSAGE_LOG_LISTING "Inventory Plugin"
#define MESSAGE_LOG_LABEL "Inventory Plugin Errors"
#define ITEM_TRACE_CHANNEL ECC_GameTraceChannel1

DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All);

class FInventoryPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
