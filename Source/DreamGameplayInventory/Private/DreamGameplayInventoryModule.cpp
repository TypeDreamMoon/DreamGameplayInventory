// Copyright Epic Games, Inc. All Rights Reserved.

#include "DreamGameplayInventoryModule.h"

#include "DreamGameplayInventoryDeveloperSettings.h"
#if WITH_EDITOR
#include "ISettingsModule.h"
#endif

#define LOCTEXT_NAMESPACE "FDreamGameplayInventoryModule"

void FDreamGameplayInventoryModule::StartupModule()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Register Setting Page
		SettingsModule->RegisterSettings(
			"Project",
			"DreamPlugin",
			"InventoryPluginSetting",
			LOCTEXT("DreamGameplayInventorySettings", "Inventory Plugin Settings"),
			LOCTEXT("InventoryPluginSetting", "Inventory Plugin Settings"),
GetMutableDefault<UDreamGameplayInventoryDeveloperSettings>());
	}
#endif
	
}

void FDreamGameplayInventoryModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDreamGameplayInventoryModule, DreamGameplayInventory)