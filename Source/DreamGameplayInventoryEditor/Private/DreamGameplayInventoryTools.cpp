#include "AssetToolsModule.h"
#include "DreamGameplayInventoryEditorModule.h"
#include "DreamGameplayInventoryEditorSettings.h"
#include "DreamGameplayInventoryEditorTools.h"
#include "DreamGameplayInventoryLog.h"
#include "KismetCompilerModule.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Classes/DreamInventoryItem.h"
#include "Engine/AssetManager.h"
#include "Classes/DreamInventoryItemType.h"
#include "Dialogs/DlgPickAssetPath.h"
#include "Editor/ClassViewer/Private/UnloadedBlueprintData.h"
#include "Engine/ObjectLibrary.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/SClassPickerDialog.h"

#define LOCTEXT_NAMESPACE "DreamGameplayInventoryEditorTools"

class FDreamInventoryItemClassFiler : public IClassViewerFilter
{
public:
	/** All children of these classes will be included unless filtered out by another setting. */
	TSet<const UClass*> AllowedChildrenOfClasses;

	/** Disallowed class flags. */
	EClassFlags DisallowedClassFlags = CLASS_None;

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		return !InClass->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;
	}

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		return !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) != EFilterReturn::Failed;
	}
};

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

UBlueprint* FDreamGameplayInventoryEditorTools::CreateBlueprintByClass(TSubclassOf<UObject> Class, FString Name, EBlueprintType BlueprintType, bool bPickClass)
{
	check(FKismetEditorUtilities::CanCreateBlueprintOfClass(Class));

	// Pre-generate a unique asset name to fill out the path picker dialog with.
	if (Name.Len() == 0)
	{
		Name = TEXT("NewBlueprint");
	}

	UClass* ChosenClass = Class;

	if (bPickClass)
	{
		FClassViewerInitializationOptions Options;
		Options.DisplayMode = EClassViewerDisplayMode::Type::TreeView;
		Options.Mode = EClassViewerMode::ClassPicker;
		Options.bShowNoneOption = false;
		Options.bExpandAllNodes = true;

		TSharedPtr<FDreamInventoryItemClassFiler> Filter = MakeShareable(new FDreamInventoryItemClassFiler);
		Options.ClassFilters.Add(Filter.ToSharedRef());

		Filter->DisallowedClassFlags = CLASS_Deprecated | CLASS_NewerVersionExists | CLASS_Abstract | CLASS_HideDropDown;
		Filter->AllowedChildrenOfClasses.Add(Class);

		const FText TitleText = LOCTEXT("CreateBlueprint_PickClass", "Pick Item Class");
		SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, Class);
	}

	// Load required modules
	FModuleManager::LoadModuleChecked<IKismetCompilerInterface>("KismetCompiler");
	FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

	// Get classes needed for blueprint creation
	UClass* BlueprintClass = nullptr;
	UClass* BlueprintGeneratedClass = nullptr;
	IKismetCompilerInterface& KismetCompilerModule = FModuleManager::GetModuleChecked<IKismetCompilerInterface>("KismetCompiler");
	KismetCompilerModule.GetBlueprintTypesForClass(ChosenClass, BlueprintClass, BlueprintGeneratedClass);

	// Generate initial package and asset names
	FString CurrentPath = FModuleManager::LoadModuleChecked<FDreamGameplayInventoryEditorModule>("DreamGameplayTaskEditor").CurrentContentBrowserPath;
	FString PackageName = CurrentPath.Right(CurrentPath.Len() - 4) / Name;
	FString UName;
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	AssetToolsModule.Get().CreateUniqueAssetName(PackageName, TEXT(""), PackageName, UName);

	UBlueprint* Blueprint = nullptr;


	// Show the asset path picker dialog
	TSharedPtr<SDlgPickAssetPath> PickAssetPathWidget =
		SNew(SDlgPickAssetPath)
		.Title(FText::FromString(TEXT("Create New Asset")))
		.DefaultAssetPath(FText::FromString(PackageName));

	if (EAppReturnType::Ok == PickAssetPathWidget->ShowModal())
	{
		// Get the full name of where we want to create the Blueprint
		FString UserPackageName = PickAssetPathWidget->GetFullAssetPath().ToString();
		FName BPName(*FPackageName::GetLongPackageAssetName(UserPackageName));

		// Check if the user inputted a valid asset name, if they did not, use the generated default name
		if (BPName == NAME_None)
		{
			// Use the defaults that were already generated.
			UserPackageName = PackageName;
			BPName = *UName;
		}

		// Then find or create the package
		UPackage* Package = CreatePackage(*UserPackageName);
		check(Package);

		// Create and initialize a new Blueprint
		Blueprint = FKismetEditorUtilities::CreateBlueprint(ChosenClass, Package, BPName, BlueprintType, BlueprintClass, BlueprintGeneratedClass, FName("LevelEditorActions"));
		if (Blueprint)
		{
			// Notify the asset registry
			FAssetRegistryModule::AssetCreated(Blueprint);

			// Mark the package dirty
			Package->MarkPackageDirty();

			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(Blueprint);
			return Blueprint;
		}
	}

	return nullptr;
}


#undef LOCTEXT_NAMESPACE
