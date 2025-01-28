#pragma once

#include "Framework/Commands/Commands.h"

class FDreamGameplayInventoryEditorCommands : public TCommands<FDreamGameplayInventoryEditorCommands>
{
public:
	FDreamGameplayInventoryEditorCommands();
	virtual void RegisterCommands() override;
public:
	
	TSharedPtr<FUICommandInfo> OpenManager;
};
