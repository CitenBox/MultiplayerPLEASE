// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using UnrealBuildTool;
using System.Collections.Generic;

public class MultiplayerPLEASETarget : TargetRules
{
	public MultiplayerPLEASETarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		//bUsesSteam = true;
		
		ExtraModuleNames.Add("MultiplayerPLEASE");
	}
}
