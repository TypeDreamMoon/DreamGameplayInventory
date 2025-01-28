#include "DreamGameplayInventoryEditorCommand.h"

#include "DreamGameplayInventoryManagerStyle.h"

#define LOCTEXT_NAMESPACE "FDreamGameplayInventoryEditorCommands"

FDreamGameplayInventoryEditorCommands::FDreamGameplayInventoryEditorCommands() : TCommands<FDreamGameplayInventoryEditorCommands>(TEXT("DreamGameplayInventoryEditor"), NSLOCTEXT("Context", "DreamGameplayInventoryEditor", "DreamGameplayInventoryEditor Plugin"), NAME_None, FDreamGameplayInventoryManagerStyle::GetStyleSetName())
{
}

void FDreamGameplayInventoryEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenManager, "Open Inventory Manager", "Open the Inventory Manager", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE