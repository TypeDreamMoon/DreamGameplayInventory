#include "DreamGameplayInventoryEditorModule.h"

#include "ContentBrowserModule.h"
#include "DreamGameplayInventoryEditorCommand.h"
#include "DreamGameplayInventoryEditorSettings.h"
#include "DreamGameplayInventoryEditorTools.h"
#include "DreamGameplayInventoryManager.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"


#include "DreamGameplayInventoryManagerStyle.h"
#include "LevelEditor.h"

#define LOCTEXT_NAMESPACE "FDreamGameplayInventoryEditorModule"

void FDreamGameplayInventoryEditorModule::StartupModule()
{
	UDreamGameplayInventoryEditorSettings::Register();

	FDreamGameplayInventoryManagerStyle::Initialize();
	FDreamGameplayInventoryManagerStyle::ReloadTextures();

	//  FGlobalTabmanager::Get()->RegisterNomadTabSpawner(ManagerTabName, FOnSpawnTab::CreateRaw(this , &FDreamGameplayInventoryEditorModule::OnSpawnPluginTab))
	// .SetDisplayName(FText::FromString(FString(TEXT("Dream Gameplay Inventory Manager"))))
	// .SetGroup(WorkspaceMenu::GetMenuStructure().GetToolsCategory())
	// .SetIcon(FSlateIcon(FDreamGameplayInventoryManagerStyle::GetStyleSetName(), "DreamGameplayInventory.Icon"))
	// .SetMenuType(ETabSpawnerMenuType::Enabled);
	RegisterCommand();
	MakeCommandList();
	RegisterMenu();
}

void FDreamGameplayInventoryEditorModule::ShutdownModule()
{
	FDreamGameplayInventoryManagerStyle::Shutdown();
	// FGlobalTabmanager::Get()->UnregisterTabSpawner(ManagerTabName);
	UDreamGameplayInventoryEditorSettings::Unregister();
}

TSharedRef<SDockTab> FDreamGameplayInventoryEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(NomadTab)
		[
			SNew(SDreamGameplayInventoryManager)
		];
}

TSharedRef<SWidget> FDreamGameplayInventoryEditorModule::MakeEditorToolsMenu()
{
	FMenuBuilder MenuBuilder(true, CommandList);
	auto BufCommandList = FDreamGameplayInventoryEditorCommands::Get();
	MenuBuilder.BeginSection("InventoryTools", LOCTEXT("DreamGameplayInventoryEd", "InventoryTools"));
	{
		MenuBuilder.AddMenuEntry(BufCommandList.OpenManager);
	}

	return MenuBuilder.MakeWidget();
}

void FDreamGameplayInventoryEditorModule::RegisterCommand()
{
	if (!CommandList.IsValid())
	{
		CommandList = MakeShareable(new FUICommandList);
	}
	FDreamGameplayInventoryEditorCommands::Register();
}

void FDreamGameplayInventoryEditorModule::MakeCommandList()
{
	auto EditorCommand = FDreamGameplayInventoryEditorCommands::Get();

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(GetInventoryManagerName(),
	                                                  FOnSpawnTab::CreateRaw(this, &FDreamGameplayInventoryEditorModule::OnSpawnPluginTab))
	                        .SetDisplayName(FText::FromString(TEXT("DreamInventoryManager")))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden)
	                        .SetIcon(FSlateIcon(
		                        FSlateIcon(FDreamGameplayInventoryManagerStyle::GetStyleSetName(),
		                                   TEXT("DreamGameplayInventoryEditor.DreamGameplayInventoryManager.TabIcon"))));

	CommandList->MapAction(
		EditorCommand.OpenManager,
		FExecuteAction::CreateLambda([this]()
			{
				FGlobalTabmanager::Get()->TryInvokeTab(GetInventoryManagerName());
			}
		));
}

FName FDreamGameplayInventoryEditorModule::GetInventoryManagerName()
{
	static FName DreamGameplayInventoryEditorManagerWindowName = FName(TEXT("DreamInventoryManager"));
	return DreamGameplayInventoryEditorManagerWindowName;
}

void FDreamGameplayInventoryEditorModule::RegisterMenu()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<FExtender> Extender = MakeShareable(new FExtender);
	Extender->AddMenuExtension(
		"Tools",
		EExtensionHook::After,
		CommandList,
		FMenuExtensionDelegate::CreateRaw(this, &FDreamGameplayInventoryEditorModule::MakeMenu));

	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(Extender);
}

void FDreamGameplayInventoryEditorModule::MakeMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection("DGIT_Window", FText::FromString("DGIT_Window"));

	auto Commands = FDreamGameplayInventoryEditorCommands::Get();

	MenuBuilder.AddMenuEntry(Commands.OpenManager, FName("Open Manager"), FText::FromString("Open Manager"));

	MenuBuilder.EndSection();
}

void FDreamGameplayInventoryEditorModule::RegisterContentBrowserPathChange()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	ContentBrowserModule.GetOnAssetPathChanged().AddLambda([this](const FString& Path)
	{
		CurrentContentBrowserPath = Path;
	});
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDreamGameplayInventoryEditorModule, DreamGameplayInventoryEditor)
