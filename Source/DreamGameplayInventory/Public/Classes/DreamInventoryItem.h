// Copyright © Dream Moon Studio . Dream Moon All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "DreamGameplayInventoryCommon.h"
#include "UObject/Object.h"
#include "DreamInventoryItem.generated.h"

class UDreamInventoryBase;

/**
 * Dream Gameplay Inventory Item
 */
UCLASS(Blueprintable)
class DREAMGAMEPLAYINVENTORY_API UDreamInventoryItem : public UObject
{
	GENERATED_BODY()
public:
	~UDreamInventoryItem();

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemSimpleDelegate, UDreamInventoryItem*, Item);

	UPROPERTY(BlueprintAssignable)
	FItemSimpleDelegate OnItemUpdate;
	
public:
	/**
	 * 当物品使用时
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnUseItem")
	void BP_OnUseItem();

	/**
	 * 当物品数值状态更新时
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnItemChanged")
	void BP_OnItemChanged();

	/**
	 * 当物品初始化时
	 * @param Owner 管理此物品的Inventory组件
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnInitialize")
	void BP_OnInitialize(UDreamInventoryBase* Owner);

	/**
	 * 当物品从Inventory中移除时
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnRemoveFromInventory")
	void BP_OnRemoveFromInventory();

public:
	/**
	 * Item Information
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
	FDreamInventoryItemDisplayInformation Information;

	/**
	 * Item Value Data
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
	FDreamInventoryItemValue ValueData;

	/**
	 * Is Empty Slot.
	 * - This property is set only at initialization time and can only be used for SlotComponent
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Data)
	bool bIsEmptySlot;

	/**
	 * Item Index
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Data)
	int Index;

	/**
	 * Item Owner Component
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Data)
	UDreamInventoryBase* OwnerComponent;
	
public:
	/**
	 * 初始化物品对象
	 * @param InOwnerComponent 管理物品的Inventory组件
	 * @param InItemIndex 当前物品的Index
	 * @return 当前物品对象
	 */
	UFUNCTION(BlueprintCallable, Category = Functions)
	UDreamInventoryItem* Initialize(UDreamInventoryBase* InOwnerComponent, int InItemIndex);

	// 获取是否能叠加
	UFUNCTION(BlueprintPure, Category = Functions)
	bool GetCanStack() const { return ValueData.bCanStack; }

	// 获取是否无上限叠加
	UFUNCTION(BlueprintPure, Category = Functions)
	bool GetCanUnlimited() const { return ValueData.bCanUnlimited; }

	// 获取当前物品数量
	UFUNCTION(BlueprintPure, Category = Functions)
	int GetCurrentCount() const { return ValueData.Count; }

	// 获取最大数量
	UFUNCTION(BlueprintPure, Category = Functions)
	int GetMaxCount() const { return ValueData.MaxCount; }

	// 获取索引
	UFUNCTION(BlueprintPure, Category = Functions)
	int GetItemIndex() const { return Index; }

	UFUNCTION(BlueprintPure, Category = Functions)
	FDreamInventoryItemDisplayInformation GetInformation() const { return Information; }

	// 获取类型
	UFUNCTION(BlueprintPure, Category = Functions)
	UDreamInventoryItemType* GetType() const { return Information.Type; }

	// 获取等级
	UFUNCTION(BlueprintPure, Category = Functions)
	UDreamInventoryItemLevel* GetLevel() const { return Information.Level; }

	// 获取当前是否能叠加
	UFUNCTION(BlueprintPure, Category = Functions)
	bool GetWhetherStackIsCurrentlyAvailable() const
	{
		return GetCanStack()
			       ? (GetCanUnlimited() ? true : (GetCurrentCount() < GetMaxCount()))
			       : false;
	}

	// 获取当前物品的拥有组件
	UFUNCTION(BlueprintPure, Category = Functions)
	UDreamInventoryBase* GetOwnerComponent() const { return OwnerComponent; }


	/**
	 * 使用全部物品
	 */
	UFUNCTION(BlueprintCallable, Category = Functions)
	void UseAllItem();

	/**
	 * 使用InCount个物品
	 * @param InCount 使用数量
	 * @return 未使用的数量
	 */
	UFUNCTION(BlueprintCallable, Category = Functions)
	int UseItem(int InCount = 1);

	/**
	 * 添加物品
	 * @param InCount 添加物品数量
	 */
	UFUNCTION(BlueprintCallable, Category = Functions)
	int AddItem(int InCount);

	/**
	 * 移除物品
	 * @param InCount 移除物品数量
	 * @return 未移除的数量
	 */
	UFUNCTION(BlueprintCallable, Category = Functions)
	int RemoveItem(int InCount);

	/**
	 * 移除全部物品
	 */
	UFUNCTION(BlueprintCallable, Category = Functions)
	void RemoveAllItem();

	/**
	 * 移除单个物品
	 */
	UFUNCTION(BlueprintCallable, Category = Functions)
	void RemoveItemSingle();

	/**
	 * Is Empty Slot
	 * @return 当前是否为空插槽 
	 */
	UFUNCTION(BlueprintPure, Category = Functions)
	bool IsEmptySlot() const { return bIsEmptySlot; }

public:
	/**
	 * 设置当前是否为空插槽
	 * @param bState 新的插槽模式
	 */
	void SetIsEmptySlot(bool bState);

	// 获取当前是否为空插槽
	bool GetIsEmptySlot() const { return bIsEmptySlot; }

private:
	virtual void Item_RemoveSingle();
	virtual void Item_RemoveAll();
	virtual int Item_Remove(int InRemoveCount);
	virtual void Item_UseAll();
	virtual int Item_Use(int InUseCount);
	virtual void Item_CheckCanBeginDestory();
public:
	void SetValueData(FDreamInventoryItemValue InData);

	/** UObject **/
	virtual UWorld* GetWorld() const override; 
};
