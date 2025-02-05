// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DreamInventoryItemLevel.generated.h"

/**
 * 
 */
UCLASS()
class DREAMGAMEPLAYINVENTORY_API UDreamInventoryItemLevel : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor LevelColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 SortPriority = UINT8_MAX;
};
