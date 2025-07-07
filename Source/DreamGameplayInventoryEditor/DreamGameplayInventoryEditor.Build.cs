using UnrealBuildTool;

public class DreamGameplayInventoryEditor : ModuleRules
{
    public DreamGameplayInventoryEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "DreamGameplayInventory", 
                "WorkspaceMenuStructure",
                "Projects",
                "DeveloperSettings",
                "InputCore",
                "UnrealEd", 
                "Blutility",
            }
        );
    }
}