// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DreamGameplayInventoryEditorSettings.generated.h"

/**
 * 
 */
UCLASS(DefaultConfig, Config = DreamGameplayInventoryEditor)
class DREAMGAMEPLAYINVENTORYEDITOR_API UDreamGameplayInventoryEditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	virtual FName GetCategoryName() const override { return TEXT("DreamPlugin"); }
	virtual FName GetContainerName() const override { return TEXT("Editor"); }
	virtual FName GetSectionName() const override { return TEXT("InventoryEditorSetting"); }

public:
	static UDreamGameplayInventoryEditorSettings* Get();

	static void Register();
	static void Unregister();

public:
	UPROPERTY(EditAnywhere, Config, Category = "Manager", Meta = (LongPackageName))
	TArray<FDirectoryPath> ManagerLoadObjectPaths;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Config, Category = "Version")
	FName ManagerVersion = FName(TEXT("0.1.0"));

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Config, Category = "Version")
	FName DebuggerVersion = FName(TEXT("0.0.1"));
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Config, Category = "Version")
	FName PluginVersion = FName(TEXT("0.1.0"));
};
