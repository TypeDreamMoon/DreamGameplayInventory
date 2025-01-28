// Copyright © Dream Moon Studio . Dream Moon All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DreamGameplayInventoryDeveloperSettings.generated.h"

class UDreamInventoryItem;
/**
 *
 */
UCLASS(DefaultConfig, Config = DreamGameplayInventory)
class DREAMGAMEPLAYINVENTORY_API UDreamGameplayInventoryDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	static UDreamGameplayInventoryDeveloperSettings* Get();
public:
	/** Gets the settings container name for the settings, either Project or Editor */
	virtual FName GetContainerName() const override { return TEXT("Project"); }
	/** Gets the category for the settings, some high level grouping like, Editor, Engine, Game...etc. */
	virtual FName GetCategoryName() const override { return TEXT("DreamPlugin"); }
	/** The unique name for your section of settings, uses the class's FName. */
	virtual FName GetSectionName() const override { return TEXT("InventoryPluginSetting"); }
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Default, meta = (ForceInlineRow), Config)
	TMap<TSubclassOf<UDreamInventoryItem>, FGuid> ItemGuids;
public:
	TSubclassOf<UDreamInventoryItem> GetItemClassByGuid(FGuid InGuid);
	FGuid GetItemGuidByClass(TSubclassOf<UDreamInventoryItem> InClass);
};
