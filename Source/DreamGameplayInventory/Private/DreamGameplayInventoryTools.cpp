#include "DreamGameplayInventoryTools.h"

#include "Algo/Reverse.h"
#include "Classes/DreamInventoryItem.h"

FDreamInventoryFindResultSpec UDreamInventoryTools::FindObjectsToAdd(TArray<UDreamInventoryItem*> TargetArray, TSubclassOf<UDreamInventoryItem> InItemClass, int InCount, bool bReverse)
{
	// 判断是否能叠加 不可以叠加直接返回
	if (!InItemClass.GetDefaultObject()->GetCanStack())
	{
		return FDreamInventoryFindResultSpec();
	}

	TArray<UDreamInventoryItem*>& FindBuffer = TargetArray;

	if (bReverse)
	{
		Algo::Reverse(FindBuffer);
	}

	for (int i = 0; i < FindBuffer.Num(); ++i)
	{
		UDreamInventoryItem* CurrentItem = FindBuffer[i];

		if (CurrentItem->GetClass() != InItemClass)
			continue;

		// 判断当前物品能否添加
		if (CurrentItem->GetWhetherStackIsCurrentlyAvailable())
		{
			// 当物品为无限叠加时
			if (CurrentItem->GetCanUnlimited())
			{
				return FDreamInventoryFindResultSpec(true, i, 0, CurrentItem);
			}
			else
			{
				int Number = CurrentItem->GetMaxCount() - CurrentItem->GetCurrentCount();
				if ((Number - InCount) >= 0)
				{
					return FDreamInventoryFindResultSpec(true, i, 0, CurrentItem);
				}
				else
				{
					return FDreamInventoryFindResultSpec(true, i, FMath::Abs(Number - InCount), CurrentItem);
				}
			}
		}
	}

	return FDreamInventoryFindResultSpec();
}

FDreamInventoryFindResultSpec UDreamInventoryTools::FindObjectsToRemove(TArray<UDreamInventoryItem*> TargetArray, TSubclassOf<UDreamInventoryItem> InItemClass, int InCount, bool bReverse)
{
	TArray<UDreamInventoryItem*> FindBuffer = TargetArray;

	if (bReverse)
	{
		Algo::Reverse(FindBuffer);
	}

	for (int i = 0; i < FindBuffer.Num(); ++i)
	{
		UDreamInventoryItem* CurrentItem = FindBuffer[i];

		if (CurrentItem->GetClass() != InItemClass)
			continue;
		if ((CurrentItem->GetCurrentCount() - InCount) > 0)
		{
			return FDreamInventoryFindResultSpec(true, i, 0, CurrentItem);
		}
		else
		{
			return FDreamInventoryFindResultSpec(true, i, FMath::Abs(CurrentItem->GetCurrentCount() - InCount), CurrentItem);
		}
	}

	return FDreamInventoryFindResultSpec();
}
