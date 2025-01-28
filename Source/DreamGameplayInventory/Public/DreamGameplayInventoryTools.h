#pragma once

#include "CoreMinimal.h"
#include "DreamGameplayInventoryCommon.h"
#include "DreamGameplayInventoryTools.generated.h"

/**
 * Dream Inventory Tools
 */
UCLASS()
class DREAMGAMEPLAYINVENTORY_API UDreamInventoryTools : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * 查找要添加的对象
	 * @param TargetArray 目标数组
	 * @param InItemClass 查找的类
	 * @param InCount 要添加的数量
	 * @param bReverse 是否反向查找
	 * @return 查找结果
	 */
	static FDreamInventoryFindResultSpec FindObjectsToAdd(TArray<UDreamInventoryItem*> TargetArray, TSubclassOf<UDreamInventoryItem> InItemClass, int InCount, bool bReverse);

	/**
	 * 找到要移除的对象
	 * @param TargetArray 目标数组
	 * @param InItemClass 查找的类
	 * @param InCount 要移除的数量
	 * @param bReverse 是否反向查找
	 * @return 查找结果
	 */
	static FDreamInventoryFindResultSpec FindObjectsToRemove(TArray<UDreamInventoryItem*> TargetArray, TSubclassOf<UDreamInventoryItem> InItemClass, int InCount, bool bReverse);
};
