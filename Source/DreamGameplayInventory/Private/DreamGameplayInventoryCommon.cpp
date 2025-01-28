#include "DreamGameplayInventoryCommon.h"

#include "DreamGameplayInventoryDeveloperSettings.h"
#include "DreamGameplayInventoryLog.h"
#include "Classes/DreamInventoryItem.h"

FDreamInventoryItemSaveData::FDreamInventoryItemSaveData(const UDreamInventoryItem* Item)
{
	if (Item)
	{
		FGuid Tag = UDreamGameplayInventoryDeveloperSettings::Get()->GetItemGuidByClass(Item->GetClass());
		if (Tag.IsValid())
		{
			ClassGuid = Tag;
			Value = Item->ValueData;
		}
		else
		{
			DI_FLOG(Error, TEXT("Save error!!! No define Item Class Tag!!! Go to (Item Settings -> DreamPlugin -> Inventory Setting -> ItemTags) to register the class Tag."))
		}
	}
}

FDreamInventorySaveGameData::FDreamInventorySaveGameData(const TArray<UDreamInventoryItem*>& InSaveItems, EDreamInventorySaveType InSaveType)
{
	SaveType = InSaveType;
	for (auto Element : InSaveItems)
	{
		SaveGameDatas.Add(FDreamInventoryItemSaveData(Element));
	}
}
