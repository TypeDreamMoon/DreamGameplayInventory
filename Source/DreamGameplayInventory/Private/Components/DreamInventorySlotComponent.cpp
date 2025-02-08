// Copyright © Dream Moon Studio . Dream Moon All rights reserved


#include "Components/DreamInventorySlotComponent.h"

UDreamInventorySlotComponent::UDreamInventorySlotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SaveType = EDreamInventorySaveType::EDIST_Slot;
}

void UDreamInventorySlotComponent::RefreshSlot()
{
}

void UDreamInventorySlotComponent::InitializeSlot()
{
	for (int i = 0; i < ItemSlotCount; ++i)
	{
		UDreamInventoryItem* NewItem = MakeEmptySlot();
		GetInventoryData().Add(NewItem);
	}

	OnDataChanged.Broadcast(GetInventoryData());
}

bool UDreamInventorySlotComponent::SwapItemByItem(UDreamInventoryItem* TargetA, UDreamInventoryItem* TargetB)
{
	if (InventoryHasItem(TargetA) && InventoryHasItem(TargetB))
	{
		GetInventoryData().Swap(TargetA->GetItemIndex(), TargetB->GetItemIndex());
		return true;
	}

	return false;
}

bool UDreamInventorySlotComponent::SwapItemByIndex(int TargetIndexA, int TargetIndexB)
{
	if (InventoryHasItem(GetItemByIndex(TargetIndexA)) && InventoryHasItem(GetItemByIndex(TargetIndexB)))
	{
		GetInventoryData().Swap(TargetIndexA, TargetIndexB);
		return true;
	}

	return false;
}

void UDreamInventorySlotComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeSlot();
}

int UDreamInventorySlotComponent::GiveItemByClass(TSubclassOf<UDreamInventoryItem> InClass, int InCount)
{
	if (!InClass || InCount <= 0)
	{
		return InCount; // 如果类无效或数量无效，直接返回剩余数量
	}

	TArray<int> FindIndexArray;
	bool bFound = false;

	// 查找可以添加物品的插槽
	FindsSlotsThatCanBeAdded(InClass, FindIndexArray, bFound);

	if (!bFound || FindIndexArray.Num() == 0)
	{
		return InCount; // 没有可用插槽，返回剩余数量
	}

	int RemainingCount = InCount;

	for (int FindIndex : FindIndexArray)
	{
		if (RemainingCount <= 0)
		{
			break; // 如果已分配完所有物品，退出循环
		}

		UDreamInventoryItem* CurrentItem = GetInventoryData()[FindIndex];

		if (CurrentItem->IsEmptySlot())
		{
			// 如果是空槽，创建新物品
			UDreamInventoryItem* NewItem = NewItemByClass(InClass);
			NewItem->Initialize(this, FindIndex);

			if (NewItem)
			{
				GetInventoryData()[FindIndex] = NewItem;

				// 如果物品不可堆叠，每个插槽只能存储一个
				if (!NewItem->GetCanStack())
				{
					RemainingCount -= 1; // 每次分配一个
				}
				else
				{
					// 可堆叠物品，尝试添加所有剩余数量
					int Added = NewItem->AddItem(RemainingCount);
					RemainingCount -= (RemainingCount - Added);
				}
			}
		}
		else if (CurrentItem->GetCanStack() && CurrentItem->GetWhetherStackIsCurrentlyAvailable())
		{
			// 如果当前槽有可堆叠的物品，尝试添加
			int Added = CurrentItem->AddItem(RemainingCount);
			RemainingCount -= (RemainingCount - Added);
		}
	}

	OnDataChanged.Broadcast(GetInventoryData()); // 通知数据发生变化

	return RemainingCount; // 返回未能分配的数量
}

int UDreamInventorySlotComponent::RemoveItemByClass(TSubclassOf<UDreamInventoryItem> InClass, int InCount)
{
	// 输入验证
	if (!InClass || InCount <= 0)
	{
		return InCount; // 无效输入，直接返回剩余数量
	}

	int RemainingCount = InCount;

	// 遍历库存，查找目标类型物品
	for (UDreamInventoryItem* Item : GetInventoryData())
	{
		if (RemainingCount <= 0)
		{
			break; // 如果已完成移除目标数量，退出循环
		}

		// 检查物品是否属于指定类型
		if (Item && Item->IsA(InClass))
		{
			// 使用 RemoveItemByItem 移除物品
			RemainingCount = RemoveItemByItem(Item, RemainingCount);
		}
	}

	// 广播数据变化事件
	if (RemainingCount < InCount) // 仅在移除过物品时广播
	{
		OnDataChanged.Broadcast(GetInventoryData());
	}

	return RemainingCount; // 返回未能移除的数量
}

int UDreamInventorySlotComponent::RemoveItemByItem(UDreamInventoryItem* InItem, int InCount)
{
	// 输入验证
	if (!InItem || InCount <= 0)
	{
		return InCount; // 无效输入，直接返回剩余数量
	}

	// 检查物品是否在库存中
	if (!InventoryHasItem(InItem))
	{
		return InCount; // 物品不在库存中，直接返回剩余数量
	}

	// 当前物品的数量
	int CurrentCount = InItem->GetCurrentCount();

	if (CurrentCount <= InCount)
	{
		// 如果当前物品数量小于或等于要移除的数量
		InItem->RemoveAllItem(); // 完全移除物品
		OnRemoveItem.Broadcast(InItem); // 触发移除事件

		// 将槽标记为空
		int ItemIndex = InItem->GetItemIndex();
		GetInventoryData()[ItemIndex] = MakeEmptySlot();

		// 返回剩余的未移除数量（需要移除的数量 - 实际移除数量）
		OnDataChanged.Broadcast(GetInventoryData());
		return InCount - CurrentCount;
	}
	else
	{
		// 部分移除物品
		int Removed = InItem->RemoveItem(InCount);
		OnRemoveItem.Broadcast(InItem); // 触发移除事件

		OnDataChanged.Broadcast(GetInventoryData());
		return InCount - Removed; // 返回未能移除的数量
	}
}

int UDreamInventorySlotComponent::RemoveItemByIndex(int InIndex, int InCount)
{
	// 输入验证：检查索引是否有效
	if (InIndex < 0 || InIndex >= GetInventoryData().Num() || InCount <= 0)
	{
		return InCount; // 索引或数量无效，直接返回剩余数量
	}

	// 获取物品
	UDreamInventoryItem* TargetItem = GetInventoryData()[InIndex];

	// 检查物品是否为空或无效
	if (!TargetItem || TargetItem->IsEmptySlot())
	{
		return InCount; // 如果槽为空或物品无效，返回剩余数量
	}

	// 获取当前物品的数量
	int CurrentCount = TargetItem->GetCurrentCount();

	if (CurrentCount <= InCount)
	{
		// 如果当前物品数量小于或等于要移除的数量
		TargetItem->RemoveAllItem(); // 完全移除物品
		OnRemoveItem.Broadcast(TargetItem); // 触发移除事件

		// 将槽重置为空
		GetInventoryData()[InIndex] = MakeEmptySlot();

		// 广播数据变化
		OnDataChanged.Broadcast(GetInventoryData());

		// 返回剩余未移除的数量
		return InCount - CurrentCount;
	}
	else
	{
		// 部分移除物品
		int Removed = TargetItem->RemoveItem(InCount);
		OnRemoveItem.Broadcast(TargetItem); // 触发移除事件

		// 广播数据变化
		OnDataChanged.Broadcast(GetInventoryData());

		// 返回未移除的数量
		return InCount - Removed;
	}
}

int UDreamInventorySlotComponent::UseItemByClass(TSubclassOf<UDreamInventoryItem> InClass, int InCount)
{
	// 输入验证
	if (!InClass || InCount <= 0)
	{
		return InCount; // 无效输入，直接返回剩余数量
	}

	int RemainingCount = InCount;

	// 遍历库存，查找匹配的物品
	for (UDreamInventoryItem* Item : GetInventoryData())
	{
		if (RemainingCount <= 0)
		{
			break; // 已完成消耗，退出循环
		}

		// 检查物品是否匹配类型
		if (Item && Item->IsA(InClass))
		{
			// 使用 UseItemByItem 消耗物品
			RemainingCount = UseItemByItem(Item, RemainingCount);
		}
	}

	// 如果有物品被消耗，广播数据变化事件
	if (RemainingCount < InCount)
	{
		OnDataChanged.Broadcast(GetInventoryData());
	}

	return RemainingCount; // 返回未能消耗的数量
}

int UDreamInventorySlotComponent::UseItemByItem(UDreamInventoryItem* InItem, int InCount)
{
	// 输入验证
	if (!InItem || InCount <= 0)
	{
		return InCount; // 无效输入，直接返回剩余数量
	}

	// 检查物品是否在库存中
	if (!InventoryHasItem(InItem))
	{
		return InCount; // 物品不属于当前库存，返回剩余数量
	}

	// 当前物品的数量
	int CurrentCount = InItem->GetCurrentCount();

	if (CurrentCount <= InCount)
	{
		// 如果当前物品数量小于或等于要使用的数量
		InItem->UseAllItem(); // 完全使用物品
		OnUseItem.Broadcast(InItem); // 触发使用事件

		// 将槽设置为空
		int ItemIndex = InItem->GetItemIndex();
		GetInventoryData()[ItemIndex] = MakeEmptySlot();

		// 广播数据变化事件
		OnDataChanged.Broadcast(GetInventoryData());

		// 返回剩余未使用的数量（需要使用的数量 - 实际使用的数量）
		return InCount - CurrentCount;
	}
	else
	{
		// 部分使用物品
		int Used = InItem->UseItem(InCount);
		OnUseItem.Broadcast(InItem); // 触发使用事件

		// 广播数据变化事件
		OnDataChanged.Broadcast(GetInventoryData());

		// 返回未使用的数量
		return InCount - Used;
	}
}

int UDreamInventorySlotComponent::UseItemByIndex(int InIndex, int InCount)
{
	// 输入验证
	if (InIndex < 0 || InIndex >= GetInventoryData().Num() || InCount <= 0)
	{
		return InCount; // 索引或数量无效，直接返回剩余数量
	}

	// 获取物品
	UDreamInventoryItem* TargetItem = GetInventoryData()[InIndex];

	// 检查物品是否为空或无效
	if (!TargetItem || TargetItem->IsEmptySlot())
	{
		return InCount; // 如果槽为空或物品无效，返回剩余数量
	}

	// 调用 UseItemByItem 执行消耗逻辑
	return UseItemByItem(TargetItem, InCount);
}

UDreamInventoryItem* UDreamInventorySlotComponent::MakeEmptySlot()
{
	UDreamInventoryItem* Buffer = NewObject<UDreamInventoryItem>(UDreamInventoryItem::StaticClass());
	Buffer->SetIsEmptySlot(true);

	return Buffer;
}

void UDreamInventorySlotComponent::FindEmptySlot(int& FoundIndex, bool& bFound)
{
	for (int i = 0; i < GetInventoryData().Num(); ++i)
	{
		if (GetInventoryData()[i]->IsEmptySlot())
		{
			FoundIndex = i;
			bFound = true;
			return;
		}
	}

	FoundIndex = -1;
	bFound = false;
}

bool UDreamInventorySlotComponent::SplitItem(int SourceIndex, int SplitCount)
{
	// 检查输入有效性
	if (SourceIndex < 0 || SourceIndex >= GetInventoryData().Num() || SplitCount <= 0)
	{
		return false; // 无效的输入
	}

	// 获取源物品
	UDreamInventoryItem* SourceItem = GetInventoryData()[SourceIndex];
	if (!SourceItem || SourceItem->IsEmptySlot() || !SourceItem->GetCanStack())
	{
		return false; // 源槽无效、为空或物品不可堆叠
	}

	// 检查是否有足够的数量进行拆分
	if (SourceItem->GetCurrentCount() < SplitCount)
	{
		return false; // 数量不足，无法拆分
	}

	// 查找一个空槽
	int EmptySlotIndex = -1;
	bool bFoundEmptySlot = false;
	FindEmptySlot(EmptySlotIndex, bFoundEmptySlot);

	if (!bFoundEmptySlot || EmptySlotIndex == -1)
	{
		return false; // 没有可用的空槽
	}

	// 创建拆分的物品
	UDreamInventoryItem* NewItem = NewObject<UDreamInventoryItem>(SourceItem->GetClass());
	NewItem->Initialize(this, EmptySlotIndex); // 初始化新物品
	NewItem->SetValueData(SourceItem->ValueData); // 复制原物品的属性

	// 调整新物品的数量
	NewItem->AddItem(SplitCount);

	// 减少源物品的数量
	SourceItem->RemoveItem(SplitCount);

	// 将新物品放置到空槽
	GetInventoryData()[EmptySlotIndex] = NewItem;

	// 广播数据变化事件
	OnDataChanged.Broadcast(GetInventoryData());

	return true; // 拆分成功
}

void UDreamInventorySlotComponent::FindsSlotsThatCanBeAdded(TSubclassOf<UDreamInventoryItem> Class, TArray<int>& FoundIndex, bool& bFound)
{
	FoundIndex.Empty();
	bFound = false;
	for (int i = 0; i < GetInventoryData().Num(); ++i)
	{
		if (GetInventoryData()[i]->IsEmptySlot())
		{
			FoundIndex.Add(i);
			bFound = true;
		}
		else
		{
			if (GetInventoryData()[i]->GetWhetherStackIsCurrentlyAvailable())
			{
				FoundIndex.Add(i);
				bFound = true;
			}
			else
			{
				continue;
			}
		}
	}
	return;
}
