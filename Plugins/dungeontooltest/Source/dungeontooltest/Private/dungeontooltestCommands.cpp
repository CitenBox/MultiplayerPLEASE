// Copyright Epic Games, Inc. All Rights Reserved.

#include "dungeontooltestCommands.h"

#define LOCTEXT_NAMESPACE "FdungeontooltestModule"

void FdungeontooltestCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "dungeontooltest", "Bring up dungeontooltest window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
