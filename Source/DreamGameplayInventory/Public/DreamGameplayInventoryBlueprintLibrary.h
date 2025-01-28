// Copyright © Dream Moon Studio . Dream Moon All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "DreamGameplayInventoryCommon.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DreamGameplayInventoryBlueprintLibrary.generated.h"

class UDreamInventorySlotComponent;
class UDreamInventoryComponent;
class UDreamInventoryBase;
class UDreamInventoryItemType;
class UDreamInventoryItem;

/**
 * 
 */
UCLASS()
class DREAMGAMEPLAYINVENTORY_API UDreamGameplayInventoryBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Dream Gameplay Inventory Blueprint Library")
	static UDreamInventoryBase* GetDreamGameplayInventory(AActor* Target);

	UFUNCTION(BlueprintPure, Category = "Dream Gameplay Inventory Blueprint Library")
	static UDreamInventoryComponent* AsDreamGameplayInventoryComponent(UDreamInventoryBase* Inventory);

	UFUNCTION(BlueprintPure, Category = "Dream Gameplay Inventory Blueprint Library")
	static UDreamInventorySlotComponent* AsDreamSlotInventoryComponent(UDreamInventoryBase* Inventory);
	
public:
	/**
	 * 通过KeyWord筛选物品
	 * @param Target 目标数组
	 * @param KeyWord 筛选的关键字 在Item类里面配置
	 * @param SearchCase 是否区分大小写
	 * @return 筛选结果
	 */
	UFUNCTION(BlueprintCallable, Category = "Dream Gameplay Inventory Blueprint Library")
	static TArray<UDreamInventoryItem*> FilterItemByKeyWord(const TArray<UDreamInventoryItem*>& Target, FString KeyWord, ESearchCase::Type SearchCase);

	/**
	 * 通过类型筛选物品
	 * @param Target 目标数组
	 * @param Type 筛选类型
	 * @return 筛选结果
	 */
	UFUNCTION(BlueprintCallable, Category = "Dream Gameplay Inventory Blueprint Library")
	static TArray<UDreamInventoryItem*> FilterItemByType(const TArray<UDreamInventoryItem*>& Target, UDreamInventoryItemType* Type);

	/**
	 * 通过物品类筛选物品
	 * @param Target 目标数组
	 * @param Class 筛选类
	 * @param bIncludeSubClass 是否包含子类
	 * @return 筛选结果
	 */
	UFUNCTION(BlueprintCallable, Category = "Dream Gameplay Inventory Blueprint Library")
	static TArray<UDreamInventoryItem*> FilterItemByClass(const TArray<UDreamInventoryItem*>& Target, TSubclassOf<UDreamInventoryItem> Class, bool bIncludeSubClass);
};