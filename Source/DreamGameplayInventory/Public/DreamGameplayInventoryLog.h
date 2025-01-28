#pragma once

#include "Engine.h"

DREAMGAMEPLAYINVENTORY_API DECLARE_LOG_CATEGORY_EXTERN(LogDreamGameplayInventory, All, All);

#define DI_LOG(V, F, ...) UE_LOG(LogDreamGameplayInventory, V, F , ##__VA_ARGS__);
#define DI_FLOG(V, F, ...) UE_LOG(LogDreamGameplayInventory, V, TEXT("%hs - " F), __FUNCTION__, ##__VA_ARGS__);