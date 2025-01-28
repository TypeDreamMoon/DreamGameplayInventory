#pragma once

#include "CoreMinimal.h"

class FDreamGameplayInventoryEditorTools
{
private:
	static bool bIsLoadedMemory;

public:
	static void LoadAssetToMemory(UClass* Class);
	static void ForceLoadAssetToMemory(UClass* Class);
	static TArray<FAssetData> GetAssetData(UClass* Class);
	static TArray<FString> Conv_DirectoryToString(TArray<FDirectoryPath> Paths);
};
