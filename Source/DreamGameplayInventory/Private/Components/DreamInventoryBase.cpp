// Copyright © Dream Moon Studio . Dream Moon All rights reserved


#include "Components/DreamInventoryBase.h"

#include "DreamGameplayInventoryDeveloperSettings.h"
#include "DreamGameplayInventoryLog.h"
#include "Classes/DreamInventoryItem.h"
#include "Classes/DreamInventoryItemType.h"
#include "Classes/DreamInventoryItemLevel.h"

#define CALL_DATA_CHANGED() OnDataChanged.Broadcast(GetInventoryData());
#define CALL_DATA_REMOVED() OnRemoveFromData.Broadcast();
#define CALL_LOADED() OnLoaded.Broadcast();
#define CALL_INDEX_REFRESH() OnRefreshIndex.Broadcast();


void UDreamInventoryBase::SortInventoryByType(bool bReverse, bool bSafe, bool bSameTypeSortByQuantity, bool bCountSortReverse)
{
	if (bReverse)
	{
		if (bSafe)
			GetInventoryData().StableSort([bSameTypeSortByQuantity, bCountSortReverse](const UDreamInventoryItem& A, const UDreamInventoryItem& B)
			{
				if ((A.GetType() == B.GetType()) && bSameTypeSortByQuantity)
				{
					if (bCountSortReverse)
						return A.GetCurrentCount() > B.GetCurrentCount();
					else
						return A.GetCurrentCount() < B.GetCurrentCount();
				}
				else
				{
					return A.GetType()->SortPriority < B.GetType()->SortPriority;					
				}
			});
		else
			GetInventoryData().Sort([bSameTypeSortByQuantity, bCountSortReverse](const UDreamInventoryItem& A, const UDreamInventoryItem& B)
			{
				if ((A.GetType() == B.GetType()) && bSameTypeSortByQuantity)
				{
					if (bCountSortReverse)
						return A.GetCurrentCount() > B.GetCurrentCount();
					else
						return A.GetCurrentCount() < B.GetCurrentCount();
				}
				else
				{
					return A.GetType()->SortPriority < B.GetType()->SortPriority;
				}
			});
	}
	else
	{
		if (bSafe)
			GetInventoryData().StableSort([bSameTypeSortByQuantity, bCountSortReverse](const UDreamInventoryItem& A, const UDreamInventoryItem& B)
			{
				if ((A.GetType() == B.GetType()) && bSameTypeSortByQuantity)
				{
					if (bCountSortReverse)
						return A.GetCurrentCount() > B.GetCurrentCount();
					else
						return A.GetCurrentCount() < B.GetCurrentCount();
				}
				else
				{
					return A.GetType()->SortPriority > B.GetType()->SortPriority;
				}
			});
		else
			GetInventoryData().Sort([bSameTypeSortByQuantity, bCountSortReverse](const UDreamInventoryItem& A, const UDreamInventoryItem& B)
			{
				if ((A.GetType() == B.GetType()) && bSameTypeSortByQuantity)
				{
					if (bCountSortReverse)
						return A.GetCurrentCount() > B.GetCurrentCount();
					else
						return A.GetCurrentCount() < B.GetCurrentCount();
				}
				else
				{
					return A.GetType()->SortPriority > B.GetType()->SortPriority;
				}
			});
	}

	RefreshIndex();
}

void UDreamInventoryBase::SortInventoryByLevel(bool bReverse, bool bSafe, bool bSameTypeSortByQuantity, bool bCountSortReverse)
{
	if (bReverse)
	{
		if (bSafe)
			GetInventoryData().StableSort([bSameTypeSortByQuantity, bCountSortReverse](const UDreamInventoryItem& A, const UDreamInventoryItem& B)
			{
				if ((A.GetLevel() == B.GetLevel()) && bSameTypeSortByQuantity)
				{
					if (bCountSortReverse)
						return A.GetCurrentCount() > B.GetCurrentCount();
					else
						return A.GetCurrentCount() < B.GetCurrentCount();
				}
				else
				{
					return A.GetLevel()->SortPriority < B.GetLevel()->SortPriority;					
				}
			});
		else
			GetInventoryData().Sort([bSameTypeSortByQuantity, bCountSortReverse](const UDreamInventoryItem& A, const UDreamInventoryItem& B)
			{
				if ((A.GetLevel() == B.GetLevel()) && bSameTypeSortByQuantity)
				{
					if (bCountSortReverse)
						return A.GetCurrentCount() > B.GetCurrentCount();
					else
						return A.GetCurrentCount() < B.GetCurrentCount();
				}
				else
				{
					return A.GetLevel()->SortPriority < B.GetLevel()->SortPriority;
				}
			});
	}
	else
	{
		if (bSafe)
			GetInventoryData().StableSort([bSameTypeSortByQuantity, bCountSortReverse](const UDreamInventoryItem& A, const UDreamInventoryItem& B)
			{
				if ((A.GetLevel() == B.GetLevel()) && bSameTypeSortByQuantity)
				{
					if (bCountSortReverse)
						return A.GetCurrentCount() > B.GetCurrentCount();
					else
						return A.GetCurrentCount() < B.GetCurrentCount();
				}
				else
				{
					return A.GetLevel()->SortPriority > B.GetLevel()->SortPriority;
				}
			});
		else
			GetInventoryData().Sort([bSameTypeSortByQuantity, bCountSortReverse](const UDreamInventoryItem& A, const UDreamInventoryItem& B)
			{
				if ((A.GetLevel() == B.GetLevel()) && bSameTypeSortByQuantity)
				{
					if (bCountSortReverse)
						return A.GetCurrentCount() > B.GetCurrentCount();
					else
						return A.GetCurrentCount() < B.GetCurrentCount();
				}
				else
				{
					return A.GetLevel()->SortPriority > B.GetLevel()->SortPriority;
				}
			});
	}

	RefreshIndex();
}

void UDreamInventoryBase::SortInventoryByCount(bool bReverse, bool bSafe)
{
	if (bReverse)
	{
		if (bSafe)
			GetInventoryData().StableSort([](const UDreamInventoryItem& A, const UDreamInventoryItem& B)
			{
				return A.GetCurrentCount() < B.GetCurrentCount();
			});
		else
			GetInventoryData().Sort([](const UDreamInventoryItem& A, const UDreamInventoryItem& B)
			{
				return A.GetCurrentCount() < B.GetCurrentCount();
			});
	}
	else
	{
		if (bSafe)
			GetInventoryData().StableSort([](const UDreamInventoryItem& A, const UDreamInventoryItem& B)
			{
				return A.GetCurrentCount() > B.GetCurrentCount();
			});
		else
			GetInventoryData().Sort([](const UDreamInventoryItem& A, const UDreamInventoryItem& B)
			{
				return A.GetCurrentCount() > B.GetCurrentCount();
			});
	}

	RefreshIndex();
}

void UDreamInventoryBase::SortInventoryByName(bool bReverse, bool bSafe)
{
	if (bReverse)
	{
		if (bSafe)
			GetInventoryData().StableSort([](const UDreamInventoryItem& A, const UDreamInventoryItem& B)
			{
				return A.GetInformation().Name.ToString() < B.GetInformation().Name.ToString();
			});
		else
			GetInventoryData().Sort([](const UDreamInventoryItem& A, const UDreamInventoryItem& B)
			{
				return A.GetInformation().Name.ToString() < B.GetInformation().Name.ToString();
			});
	}
	else
	{
		if (bSafe)
			GetInventoryData().StableSort([](const UDreamInventoryItem& A, const UDreamInventoryItem& B)
			{
				return A.GetInformation().Name.ToString() > B.GetInformation().Name.ToString();
			});
		else
			GetInventoryData().Sort([](const UDreamInventoryItem& A, const UDreamInventoryItem& B)
			{
				return A.GetInformation().Name.ToString() > B.GetInformation().Name.ToString();
			});
	}

	RefreshIndex();
}

FDreamInventorySaveGameData UDreamInventoryBase::ConstructSaveGameData()
{
	return FDreamInventorySaveGameData(GetInventoryData(), SaveType);
}

bool UDreamInventoryBase::LoadSaveGameData(FDreamInventorySaveGameData InSaveGameData)
{
	if (InSaveGameData.SaveType == SaveType)
	{
		GetInventoryData().Empty();
	
		for (auto Element : InSaveGameData.SaveGameDatas)
		{
			if (TSubclassOf<UDreamInventoryItem> Class = UDreamGameplayInventoryDeveloperSettings::Get()->GetItemClassByGuid(Element.ClassGuid))
			{
				UDreamInventoryItem* Item = NewItemByClass(Class);
				int index = GetInventoryData().Add(Item);
				Item->Initialize(this, index);
				Item->SetValueData(Element.Value);
			}
			else
			{
				DI_LOG(Error, TEXT("Class Null."));
			}
		}
	
		CALL_LOADED()
		CALL_DATA_CHANGED()
	
		DI_LOG(Log, TEXT("Load Data Success. Count: %d"), InSaveGameData.SaveGameDatas.Num());
		return true;
	}
	else
	{
		DI_LOG(Warning, TEXT("Load Data Failed. Save Type Is Not Equal."))
		return false;
	}
}

void UDreamInventoryBase::RefreshDataIndex()
{
	RefreshIndex();
}

bool UDreamInventoryBase::InventoryHasItem(UDreamInventoryItem* InItem)
{
	if (!InItem) return false;
	return GetInventoryData().Contains(InItem);
}

UDreamInventoryItem* UDreamInventoryBase::GetItemByIndex(int InIndex)
{
	if (GetInventoryData().IsValidIndex(InIndex))
	{
		return GetInventoryData()[InIndex];
	}
	else
	{
		return nullptr;
	}
}

void UDreamInventoryBase::CleanEmptyItem()
{
	for (int i = 0; i < GetInventoryData().Num(); ++i)
	{
		if (GetInventoryData()[i]->GetCurrentCount() <= 0)
		{
			GetInventoryData().RemoveAt(i);
		}
	}
	RefreshIndex();
}

void UDreamInventoryBase::RefreshIndex()
{
	for (int i = 0; i < GetInventoryData().Num(); ++i)
	{
		GetInventoryData()[i]->Initialize(this, i);
	}

	CALL_INDEX_REFRESH()
	CALL_DATA_CHANGED()
}

void UDreamInventoryBase::RemoveByIndex(int InIndex)
{
	DI_FLOG(Log, TEXT("Remove Index: %d"), InIndex);
	if (GetInventoryData().IsValidIndex(InIndex))
	{
		UDreamInventoryItem* Item = GetInventoryData()[InIndex];
		GetInventoryData().RemoveAt(InIndex);
		Item->ConditionalBeginDestroy();
		RefreshIndex();
	}
	
	CALL_DATA_REMOVED()
}

UDreamInventoryItem* UDreamInventoryBase::NewItemByClass(TSubclassOf<UDreamInventoryItem> InClass)
{
	if (InClass->IsChildOf(UDreamInventoryItem::StaticClass()))
	{
		UDreamInventoryItem* Buffer = NewObject<UDreamInventoryItem>(this, InClass);
		return Buffer;
	}
	else
	{
		DI_FLOG(Error, TEXT("Create Item Error. Class Is Not UDreamInventoryItem Child."))
		return nullptr;
	}
}
