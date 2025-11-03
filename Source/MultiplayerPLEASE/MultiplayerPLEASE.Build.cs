// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MultiplayerPLEASE : ModuleRules
{
	public MultiplayerPLEASE(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "NetCore", "UMG", "Slate", "JsonUtilities", "Json", "TrollMannsDungeonGenerator", "SlateCore", "Niagara",});
		CppStandard = CppStandardVersion.Cpp20;
	}
}
