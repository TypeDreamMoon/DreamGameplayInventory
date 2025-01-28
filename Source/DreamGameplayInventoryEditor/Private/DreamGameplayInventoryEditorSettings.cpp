// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.


#include "DreamGameplayInventoryEditorSettings.h"
#if WITH_EDITOR
#include "ISettingsModule.h"
#endif

UDreamGameplayInventoryEditorSettings* UDreamGameplayInventoryEditorSettings::Get()
{
	return GetMutableDefault<UDreamGameplayInventoryEditorSettings>();
}

void UDreamGameplayInventoryEditorSettings::Register()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Register Setting Page
		SettingsModule->RegisterSettings(
			"Editor",
			"DreamPlugin",
			"InventoryEditorSetting",
			FText::FromString(TEXT("Inventory Editor Setting")),
			FText::FromString(TEXT("Inventory Editor Setting")),
			Get()
		);
	}
#endif
}

void UDreamGameplayInventoryEditorSettings::Unregister()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Editor", "DreamPlugin", "InventoryEditorSetting");
	}
#endif
}
