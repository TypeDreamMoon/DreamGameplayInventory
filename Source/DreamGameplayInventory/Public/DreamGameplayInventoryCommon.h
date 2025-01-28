#pragma once

#include "CoreMinimal.h"
#include "DreamGameplayInventoryCommon.generated.h"

class UDreamInventoryItem;
class UDreamInventoryItemType;
struct FDreamInventoryItemValue;
struct FDreamInventoryItemDisplayInformation;

UENUM(BlueprintType)
enum class EDreamInventorySaveType : uint8
{
	EDIST_Default			= 0		UMETA(DisplayName = "Default"),
	EDIST_Slot				= 1		UMETA(DisplayName = "Slot"),
};

/**
 * FDreamInventoryItemValue
 * -Basic quantitative properties of items
 */
USTRUCT(BlueprintType)
struct FDreamInventoryItemValue
{
	GENERATED_BODY()
public:
	// 物品的重量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight = 0.0f;

	// 物品是否可以堆叠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanStack = true;

	// 物品堆叠是否无上限
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = bCanStack, EditConditionHides))
	bool bCanUnlimited = true;

	// 物品的当前数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = bCanStack, EditConditionHides))
	int Count = 0;

	// 物品的最大数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition = "bCanStack==true&&bCanUnlimited==false", EditConditionHides))
	int MaxCount = 1;
};

/**
 * FDreamInventoryItemDisplayInformation
 * -Item display information
 */
USTRUCT(BlueprintType)
struct FDreamInventoryItemDisplayInformation
{
	GENERATED_BODY()
public:
	// 物品的名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name = FText::FromString(TEXT("ItemName"));

	// 物品的描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description = FText::FromString(TEXT("ItemDescription"));

	// 物品关键字
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> KeyWords;
	
	// 物品的类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDreamInventoryItemType* Type;

	// 物品的缩略图
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;
};

USTRUCT(BlueprintType)
struct FDreamInventoryItemSaveData
{
	GENERATED_BODY()
public:
	FDreamInventoryItemSaveData() {};
	FDreamInventoryItemSaveData(const UDreamInventoryItem* Item);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid ClassGuid;

	UPROPERTY(EditAnywhere)
	FDreamInventoryItemValue Value;
};

USTRUCT(BlueprintType)
struct FDreamInventorySaveGameData
{
	GENERATED_BODY()
public:
	FDreamInventorySaveGameData() {}
	FDreamInventorySaveGameData(const TArray<UDreamInventoryItem*>& InSaveItems, EDreamInventorySaveType InSaveType);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDreamInventoryItemSaveData> SaveGameDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDreamInventorySaveType SaveType = EDreamInventorySaveType::EDIST_Default;
};

/**
 * Dream Inventory Find Result Spec
 * -bFound 是否查找到
 * -FindIndex 查找到的索引
 * -FindRemaining 查找到的对象Count - InCount剩下的数量
 * -FindItem 查找到的对象
 */
USTRUCT(BlueprintType)
struct FDreamInventoryFindResultSpec
{
	GENERATED_BODY()
public:
	FDreamInventoryFindResultSpec() : bFound(false), FindIndex(-1), FindRemaining(0), FindItem(nullptr){};
	FDreamInventoryFindResultSpec(bool InFound, int InFindIndex, int InFindRemaining, UDreamInventoryItem* InItem) :
		bFound(InFound), FindIndex(InFindIndex), FindRemaining(InFindRemaining), FindItem(InItem) {};
public:
	// 是否查找到
	bool bFound = false;
	// 查找到的索引
	int FindIndex = -1;
	// 查找到的对象Count - InCount剩下的数量 如果等于0 则可以完全加入 若不等于0 则为添加后剩余
	int FindRemaining = 0;
	// 查找到的对象
	UDreamInventoryItem* FindItem = nullptr;

public:
	bool IsFound() const { return bFound; }
	int GetFindIndex() const { return FindIndex; }
	int GetFindRemaining() const { return FindRemaining; }
	UDreamInventoryItem* GetFindItem() const { return FindItem; }
};
