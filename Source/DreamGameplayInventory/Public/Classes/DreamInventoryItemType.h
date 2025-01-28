// Copyright © Dream Moon Studio . Dream Moon All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DreamInventoryItemType.generated.h"

/**
 * 
 */
UCLASS()
class DREAMGAMEPLAYINVENTORY_API UDreamInventoryItemType : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	/**
	 * 类型的显示名称
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TypeDisplayName;

	/**
	 * 类型的内部名称
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TypeName;

	/**
	 * 排序优先级
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 SortPriority = UINT8_MAX;

	/**
	 * 类型的图标
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> TypeIcon;

	/**
	 * 类型的颜色
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor TypeColor;
};
