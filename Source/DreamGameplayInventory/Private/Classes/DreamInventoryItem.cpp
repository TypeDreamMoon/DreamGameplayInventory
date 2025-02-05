// Copyright © Dream Moon Studio . Dream Moon All rights reserved


#include "Classes/DreamInventoryItem.h"

#include "UObject/ObjectSaveContext.h"

#include "DreamGameplayInventoryDeveloperSettings.h"
#include "DreamGameplayInventoryLog.h"
#include "Components/DreamInventoryBase.h"

#define CALL_COMPONENT_DATA_CHANGE_DELEGATE() if (GetOwnerComponent()) GetOwnerComponent()->OnDataChanged.Broadcast(GetOwnerComponent()->GetInventoryData());
#define CALL_COMPONENT_ADD_ITEM_DELEGATE() if (GetOwnerComponent()) GetOwnerComponent()->OnAddItem.Broadcast(this);
#define CALL_COMPONENT_REMOVE_ITEM_DELEGATE() if (GetOwnerComponent()) GetOwnerComponent()->OnRemoveItem.Broadcast(this);
#define CALL_COMPONENT_USE_ITEM_DELEGATE() if (GetOwnerComponent()) GetOwnerComponent()->OnUseItem.Broadcast(this);

UDreamInventoryItem::~UDreamInventoryItem()
{
	DI_FLOG(Log, TEXT("%s Item Destory."), *GetInformation().Name.ToString());
}

UDreamInventoryItem* UDreamInventoryItem::Initialize(UDreamInventoryBase* InOwnerComponent, int InItemIndex)
{
	OwnerComponent = InOwnerComponent;
	Index = InItemIndex;

	if (!GetCanStack())
	{
		ValueData.Count = 1;
		ValueData.MaxCount = 1;
	}

	BP_OnInitialize(InOwnerComponent);

	return this;
}

void UDreamInventoryItem::UseAllItem()
{
	Item_UseAll();
	Item_CheckCanBeginDestory();
	CALL_COMPONENT_USE_ITEM_DELEGATE();
	CALL_COMPONENT_DATA_CHANGE_DELEGATE();
	BP_OnItemChanged();
}

int UDreamInventoryItem::UseItem(int InCount)
{
	int ResultCount = Item_Use(InCount);
	Item_CheckCanBeginDestory();
	CALL_COMPONENT_USE_ITEM_DELEGATE();
	CALL_COMPONENT_DATA_CHANGE_DELEGATE();
	BP_OnItemChanged();

	return ResultCount;
}

int UDreamInventoryItem::AddItem(int InCount)
{
	if (InCount <= 0)
	{
		DI_FLOG(Warning, TEXT("Invalid count provided to AddItem: %d"), InCount);
		return 0; // 如果数量无效，直接返回
	}

	if (!GetCanStack())
	{
		DI_FLOG(Error, TEXT("Item %s cannot stack. AddItem aborted."), *GetInformation().Name.ToString());
		return InCount; // 无法叠加的物品，返回未添加的数量
	}

	if (!GetWhetherStackIsCurrentlyAvailable())
	{
		DI_FLOG(Error, TEXT("Stacking is not available for item %s. AddItem aborted."), *GetInformation().Name.ToString());
		return InCount; // 堆叠不可用，返回未添加的数量
	}

	int RemainingCount = InCount;

	if (GetCanUnlimited())
	{
		// 如果可以无限堆叠，直接增加数量
		ValueData.Count += RemainingCount;
		RemainingCount = 0; // 表示所有数量都已添加
	}
	else
	{
		// 计算堆叠剩余容量
		int MaxAddable = GetMaxCount() - GetCurrentCount();

		if (MaxAddable > 0)
		{
			// 如果有空间添加物品
			int AddCount = FMath::Min(RemainingCount, MaxAddable); // 取剩余数量与可添加数量的最小值
			ValueData.Count += AddCount;
			RemainingCount -= AddCount; // 更新未添加的数量
		}
	}

	// 广播事件和更新
	CALL_COMPONENT_ADD_ITEM_DELEGATE();
	CALL_COMPONENT_DATA_CHANGE_DELEGATE();
	BP_OnItemChanged();

	return RemainingCount; // 返回未能添加的剩余数量
}

int UDreamInventoryItem::RemoveItem(int InCount)
{
	int ResultCount = Item_Remove(InCount);
	Item_CheckCanBeginDestory();
	CALL_COMPONENT_REMOVE_ITEM_DELEGATE();
	CALL_COMPONENT_DATA_CHANGE_DELEGATE();
	BP_OnItemChanged();
	return ResultCount;
}

void UDreamInventoryItem::RemoveAllItem()
{
	Item_RemoveAll();
	Item_CheckCanBeginDestory();
	CALL_COMPONENT_REMOVE_ITEM_DELEGATE();
	CALL_COMPONENT_DATA_CHANGE_DELEGATE();
	BP_OnItemChanged();
}

void UDreamInventoryItem::RemoveItemSingle()
{
	Item_RemoveSingle();
	Item_CheckCanBeginDestory();
	CALL_COMPONENT_REMOVE_ITEM_DELEGATE();
	CALL_COMPONENT_DATA_CHANGE_DELEGATE();
	BP_OnItemChanged();
}

void UDreamInventoryItem::SetIsEmptySlot(bool bState)
{
	bIsEmptySlot = bState;
}

void UDreamInventoryItem::Item_RemoveSingle()
{
	if (GetCanStack())
	{
		if (GetCurrentCount() - 1 <= 0)
		{
			ValueData.Count--;
		}
		else
		{
			ValueData.Count--;
		}
	}
	else
	{
		ValueData.Count--;
	}
	OnItemUpdate.Broadcast(this);
}

void UDreamInventoryItem::Item_RemoveAll()
{
	if (GetCanStack())
	{
		int num = GetCurrentCount();
		for (int i = 0; i < num; ++i)
		{
			Item_RemoveSingle();
		}
	}
	else
	{
		Item_RemoveSingle();
	}
	OnItemUpdate.Broadcast(this);
}

int UDreamInventoryItem::Item_Remove(int InRemoveCount)
{
	if (GetCanStack())
	{
		if (InRemoveCount > GetCurrentCount())
		{
			int count = GetCurrentCount();
			for (int i = 0; i < count; ++i)
			{
				Item_RemoveSingle();
			}
			OnItemUpdate.Broadcast(this);
			return InRemoveCount - GetCurrentCount();
		}
		else
		{
			for (int i = 0; i < InRemoveCount; ++i)
			{
				Item_RemoveSingle();
			}
			OnItemUpdate.Broadcast(this);
			return 0;
		}
	}
	else
	{
		Item_RemoveSingle();
		OnItemUpdate.Broadcast(this);
		return FMath::Abs(GetCurrentCount() - InRemoveCount);
	}
}

void UDreamInventoryItem::Item_UseAll()
{
	int num = GetCurrentCount();
	for (int i = 0; i < num; ++i)
	{
		BP_OnUseItem();
		Item_RemoveSingle();
	}
	OnItemUpdate.Broadcast(this);
}

int UDreamInventoryItem::Item_Use(int InUseCount)
{
	if (GetCanStack())
	{
		// 如果当前可叠加
		if (InUseCount > GetCurrentCount())
		{
			// 使用物品数量大于当前数量
			int ResultCount = InUseCount - GetCurrentCount();
			Item_UseAll();
			OnItemUpdate.Broadcast(this);
			return ResultCount;
		}
		else
		{
			// 使用物品小于当前数量
			for (int i = 0; i < InUseCount; ++i)
			{
				BP_OnUseItem();
			}
			Item_Remove(InUseCount);
			OnItemUpdate.Broadcast(this);
			return 0;
		}
	}
	else
	{
		BP_OnUseItem();
		Item_RemoveSingle();
		OnItemUpdate.Broadcast(this);
		return FMath::Abs(InUseCount - 1);
	}
}

void UDreamInventoryItem::Item_CheckCanBeginDestory()
{
	if (GetCurrentCount() <= 0)
	{
		GetOwnerComponent()->RemoveByIndex(Index);
		BP_OnRemoveFromInventory();
		OnItemUpdate.Broadcast(this);
	}
}

void UDreamInventoryItem::SetValueData(FDreamInventoryItemValue InData)
{
	ValueData = InData;
}
