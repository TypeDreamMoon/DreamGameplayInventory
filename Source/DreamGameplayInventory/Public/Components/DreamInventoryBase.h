// Copyright © Dream Moon Studio . Dream Moon All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "DreamGameplayInventoryCommon.h"
#include "Components/ActorComponent.h"
#include "DreamInventoryBase.generated.h"

class UDreamInventoryItem;

/**
 * Dream Gameplay Inventory Base
 */
UCLASS(ClassGroup=DreamComponent, Abstract)
class DREAMGAMEPLAYINVENTORY_API UDreamInventoryBase : public UActorComponent
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemDelegate, UDreamInventoryItem*, Item);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemClassDelegate, TSubclassOf<UDreamInventoryItem>, ItemClass);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAddedDelegate, TSubclassOf<UDreamInventoryItem>, ItemClass, int, Count);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemDataDelegate, const TArray<UDreamInventoryItem*>&, Items);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSimpleDelegate);

public:
	/**
	 * 物品添加时
	 */
	UPROPERTY(BlueprintAssignable)
	FOnItemAddedDelegate OnAddItem;

	/**
	 * 物品使用时
	 */
	UPROPERTY(BlueprintAssignable)
	FOnItemDelegate OnUseItem;

	/**
	 * 物品移除时
	 */
	UPROPERTY(BlueprintAssignable)
	FOnItemDelegate OnRemoveItem;

	/**
	 * 物品更新时
	 */
	UPROPERTY(BlueprintAssignable)
	FOnItemDataDelegate OnDataChanged;

	/**
	 * 当物品索引刷新时
	 */
	UPROPERTY(BlueprintAssignable)
	FOnSimpleDelegate OnRefreshIndex;

	/**
	 * 当物品从物品数据数组移除时
	 */
	UPROPERTY(BlueprintAssignable)
	FOnSimpleDelegate OnRemoveFromData;

	/**
	 * 当物品加载完成时 由 Load Save Game Data 调用
	 */
	UPROPERTY(BlueprintAssignable)
	FOnSimpleDelegate OnLoaded;

public:
	/**
	 * 物品数据
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Data)
	TArray<UDreamInventoryItem*> InventoryData;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Settings)
	EDreamInventorySaveType SaveType;

public:
	/**
	 * 获取当前Inventory内的物品数据
	 * @return 当前Inventory内的物品数据
	 */
	UFUNCTION(BlueprintPure, Category = Functions)
	TArray<UDreamInventoryItem*>& GetInventoryData() { return InventoryData; }

	/**
	 * Sort Inventory By Type
	 * @param bReverse 是否反向排序
	 * @param bSafe 是否启用归并排序
	 * @param bSameTypeSortByQuantity 如果为同类型 按数量排序
	 * @param bCountSortReverse SameTypeSortByQuantity反向排序 
	 */
	UFUNCTION(BlueprintCallable, Category = Functions)
	void SortInventoryByType(bool bReverse, bool bSafe = false, bool bSameTypeSortByQuantity = true, bool bCountSortReverse = false);

	/**
	 * Sort Inventory By Level
	 * @param bReverse 是否反向排序
	 * @param bSafe 是否启用归并排序
	 * @param bSameTypeSortByQuantity 如果为同等级 按数量排序
	 * @param bCountSortReverse SameTypeSortByQuantity反向排序
	 */
	UFUNCTION(BlueprintCallable, Category = Functions)
	void SortInventoryByLevel(bool bReverse, bool bSafe = false, bool bSameTypeSortByQuantity = true, bool bCountSortReverse = false);

	/**
	 * Sort Inventory By Count
	 * @param bReverse 是否反向排序
	 * @param bSafe 是否启用归并排序
	 */
	UFUNCTION(BlueprintCallable, Category = Functions)
	void SortInventoryByCount(bool bReverse, bool bSafe = false);

	/**
	 * Sort Inventory By Name
	 * @param bReverse 是否反向排序
	 * @param bSafe 是否启用归并排序
	 */
	UFUNCTION(BlueprintCallable, Category = Functions)
	void SortInventoryByName(bool bReverse, bool bSafe = false);

	/**
	 * Construct Save Game Data
	 * @return 构造完成的SaveGameData
	 */
	UFUNCTION(BlueprintCallable, Category = "Functions|SaveGame")
	FDreamInventorySaveGameData ConstructSaveGameData();

	/**
	 * Load Save Game Data
	 * @param InSaveGameData 将要加载的SaveGameData数据
	 */
	UFUNCTION(BlueprintCallable, Category = "Functions|SaveGame")
	bool LoadSaveGameData(FDreamInventorySaveGameData InSaveGameData);

	/**
	 * Refresh Data Index
	 * -刷新背包内物品索引
	 * -当对物品的顺序操作时 请调用此函数
	 */
	UFUNCTION(BlueprintCallable, Category = Functions)
	void RefreshDataIndex();

	/**
	 * Inventory Has Item
	 * @param InItem 要检测的物品
	 * @return 此物品是否在背包内
	 */
	UFUNCTION(BlueprintPure, Category = Functions)
	bool InventoryHasItem(UDreamInventoryItem* InItem);

	/**
	 * Get Item By Index
	 * @param InIndex 物品索引
	 * @return 物品对象
	 */
	UFUNCTION(BlueprintPure, Category = Functions)
	UDreamInventoryItem* GetItemByIndex(int InIndex);

public:
	// 清除空物品
	virtual void CleanEmptyItem();
	// 刷新索引
	virtual void RefreshIndex();
	// 使用索引从物品数据移除物品
	virtual void RemoveByIndex(int InIndex);
	// 新建物品
	virtual UDreamInventoryItem* NewItemByClass(TSubclassOf<UDreamInventoryItem> InClass);
};
