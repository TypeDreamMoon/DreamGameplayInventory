// Copyright © Dream Moon Studio . Dream Moon All rights reserved


#include "DreamGameplayInventoryDeveloperSettings.h"

#include "DreamGameplayInventoryLog.h"
#include "Classes/DreamInventoryItem.h"

UDreamGameplayInventoryDeveloperSettings* UDreamGameplayInventoryDeveloperSettings::Get()
{
	return GetMutableDefault<UDreamGameplayInventoryDeveloperSettings>();
}

TSubclassOf<UDreamInventoryItem> UDreamGameplayInventoryDeveloperSettings::GetItemClassByGuid(FGuid InGuid)
{
	for (auto Element : ItemGuids)
	{
		if (Element.Value == InGuid)
		{
			return Element.Key;
		}
	}
	
	return nullptr;
}

FGuid UDreamGameplayInventoryDeveloperSettings::GetItemGuidByClass(TSubclassOf<UDreamInventoryItem> InClass)
{
	if (FGuid* Tag = ItemGuids.Find(InClass))
	{
		return *Tag;
	}
	else
	{
		return FGuid();
	}
}
