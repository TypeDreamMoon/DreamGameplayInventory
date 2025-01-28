#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FDreamGameplayInventoryEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    TSharedRef<SDockTab> OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs);
    TSharedRef<SWidget> MakeEditorToolsMenu();
    void RegisterCommand();
    void MakeCommandList();
    
    FName GetInventoryManagerName();

    // Menu Builder
    void RegisterMenu();
    void MakeMenu(FMenuBuilder& MenuBuilder);
    
    TSharedPtr<class FUICommandList> CommandList;
};
