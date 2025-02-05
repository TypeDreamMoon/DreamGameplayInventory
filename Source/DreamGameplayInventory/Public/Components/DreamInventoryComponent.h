// Copyright © Dream Moon Studio . Dream Moon All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "DreamInventoryBase.h"
#include "DreamInventoryComponent.generated.h"


UCLASS(ClassGroup=DreamComponent, meta=(BlueprintSpawnableComponent))
class DREAMGAMEPLAYINVENTORY_API UDreamInventoryComponent : public UDreamInventoryBase
{
	GENERATED_BODY()

public:
	UDreamInventoryComponent();

public:
	/**
	 * 给予物品 (类)
	 * @param InClass 给予物品类
	 * @param Count 给予数量
	 * @param bReverse 是否反向
	 */
	UFUNCTION(BlueprintCallable, Category = Functions)
	void GiveItemByClass(TSubclassOf<UDreamInventoryItem> InClass, int Count = 1, bool bReverse = false);

	/**
	 * 移除物品 (类)
	 * @param InClass 移除的类
	 * @param Count 移除数量
	 * @param bReverse 是否反向
	 * @return 未移除的数量
	 */
	UFUNCTION(BlueprintCallable, Category = Functions)
	int RemoveItemByClass(TSubclassOf<UDreamInventoryItem> InClass, int Count = 1, bool bReverse = false);

	/**
	 * 使用物品 (类)
	 * @param InClass 使用的物品类
	 * @param Count 使用的数量
	 * @param bReverse 是否反向
	 * @return 未使用的数量
	 */
	UFUNCTION(BlueprintCallable, Category = Functions)
	int UseItemByClass(TSubclassOf<UDreamInventoryItem> InClass, int Count = 1, bool bReverse = false);

	/**
	 * 移除物品 (物品)
	 * @param InItem 移除的物品
	 * @param Count 移除数量
	 * @return 未移除数量
	 */
	UFUNCTION(BlueprintCallable, Category = Functions)
	int RemoveItemByItem(UDreamInventoryItem* InItem, int Count = 1);

	/**
	 * 使用物品 (物品)
	 * @param InItem 使用的物品
	 * @param Count 使用数量
	 * @return 未使用的数量
	 */
	UFUNCTION(BlueprintCallable, Category = Functions)
	int UseItemByItem(UDreamInventoryItem* InItem, int Count);

private:
	// 添加物品
	virtual void AddItem(TSubclassOf<UDreamInventoryItem> InClass, int Count, bool bReverse);
	// 移除物品
	virtual int RemoveItem(TSubclassOf<UDreamInventoryItem> InClass, int Count, bool bReverse);
	// 使用物品
	virtual int UseItem(TSubclassOf<UDreamInventoryItem> InClass, int Count, bool bReverse);
};
