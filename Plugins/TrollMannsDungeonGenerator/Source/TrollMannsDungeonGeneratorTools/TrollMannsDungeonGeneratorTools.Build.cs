using UnrealBuildTool;

public class TrollMannsDungeonGeneratorTools : ModuleRules
{
    public TrollMannsDungeonGeneratorTools(ReadOnlyTargetRules Target) : base(Target)
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
                "Slate",
                "TrollMannsDungeonGenerator",
            }
        );
    }
}