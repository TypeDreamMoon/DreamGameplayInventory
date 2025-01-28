#include "DreamGameplayInventoryEditorSettings.h"
#include "DreamGameplayInventoryEditorTools.h"
#include "DreamGameplayInventoryLog.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Classes/DreamInventoryItem.h"
#include "Engine/AssetManager.h"
#include "Classes/DreamInventoryItemType.h"
#include "Engine/ObjectLibrary.h"

bool FDreamGameplayInventoryEditorTools::bIsLoadedMemory = false;

void FDreamGameplayInventoryEditorTools::LoadAssetToMemory(UClass* Class)
{
	bIsLoadedMemory = true;

	UObjectLibrary* ObjectLibrary = UObjectLibrary::CreateLibrary(Class, true, GIsEditor);
	if (!ObjectLibrary)
	{
		DI_LOG(Warning, TEXT("Failed to create UObjectLibrary."));
		return;
	}

	ObjectLibrary->AddToRoot();

	TArray<FString> Paths = Conv_DirectoryToString(UDreamGameplayInventoryEditorSettings::Get()->ManagerLoadObjectPaths);

	// 加载资产元数据
	int32 AssetNum = ObjectLibrary->LoadAssetDataFromPaths(Paths, false);
	int32 BlueprintNum = ObjectLibrary->LoadBlueprintAssetDataFromPaths(Paths, false);

	DI_LOG(Log, TEXT("Loaded %d assets and %d blueprints from %d paths."), AssetNum, BlueprintNum, Paths.Num());

	ObjectLibrary->LoadAssetsFromAssetData();
	
	// 记录加载完成
	DI_LOG(Log, TEXT("Finished loading %d assets."), AssetNum + BlueprintNum);

	ObjectLibrary->RemoveFromRoot();
}

void FDreamGameplayInventoryEditorTools::ForceLoadAssetToMemory(UClass* Class)
{
	bIsLoadedMemory = false;
	LoadAssetToMemory(Class);
}

TArray<FAssetData> FDreamGameplayInventoryEditorTools::GetAssetData(UClass* Class)
{
	TArray<FAssetData> Result;

	if (Class->IsChildOf(UPrimaryDataAsset::StaticClass()))
	{
		// 加载 AssetRegistry 模块
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

		for (TObjectIterator<UDreamInventoryItemType> It; It; ++It)
		{
			// 跳过无效或临时对象
			if (It->HasAnyFlags(RF_Transient))
			{
				continue;
			}

			UDreamInventoryItemType* Type = *It;

			// 检查对象是否有效，并确保它属于某个包
			if (Type && !Type->GetOutermost()->HasAnyFlags(RF_Transient))
			{
				// 从 AssetRegistry 获取 FAssetData
				FAssetData Data = AssetRegistryModule.Get().GetAssetByObjectPath(FSoftObjectPath(Type));

				// 确保获取的 FAssetData 有效
				if (Data.IsValid())
				{
					Result.Add(Data);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to get FAssetData for object: %s"), *Type->GetName());
				}
			}
		}
	}
	else
	{
		// 使用 UObjectLibrary 获取资产
		UObjectLibrary* Library = UObjectLibrary::CreateLibrary(Class, true, GIsEditor);
		Library->AddToRoot();

		// 获取加载路径
		TArray<FString> Paths = Conv_DirectoryToString(UDreamGameplayInventoryEditorSettings::Get()->ManagerLoadObjectPaths);

		// 加载路径中的资产数据
		Library->LoadBlueprintAssetDataFromPaths(Paths);

		// 获取所有资产数据
		Library->GetAssetDataList(Result);
	}

	// 返回结果
	return Result;

}

TArray<FString> FDreamGameplayInventoryEditorTools::Conv_DirectoryToString(TArray<FDirectoryPath> Paths)
{
	TArray<FString> Out;
	for (auto& Path : Paths)
	{
		Out.Add(Path.Path);
	}
	return Out;
}




