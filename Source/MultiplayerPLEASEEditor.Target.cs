// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using UnrealBuildTool;
using System.Collections.Generic;

public class MultiplayerPLEASEEditorTarget : TargetRules
{
	public MultiplayerPLEASEEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		//bUsesSteam = true;
		
		ExtraModuleNames.Add("MultiplayerPLEASE");
	}
}
