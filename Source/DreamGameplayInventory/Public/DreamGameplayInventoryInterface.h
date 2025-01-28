// Copyright © Dream Moon Studio . Dream Moon All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DreamGameplayInventoryInterface.generated.h"

class UDreamInventoryBase;
// This class does not need to be modified.
UINTERFACE()
class UDreamGameplayInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DREAMGAMEPLAYINVENTORY_API IDreamGameplayInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Dream Gameplay Inventory")
	UDreamInventoryBase* GetInventory();
};
