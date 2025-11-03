// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "dungeontooltestStyle.h"

class FdungeontooltestCommands : public TCommands<FdungeontooltestCommands>
{
public:

	FdungeontooltestCommands()
		: TCommands<FdungeontooltestCommands>(TEXT("dungeontooltest"), NSLOCTEXT("Contexts", "dungeontooltest", "dungeontooltest Plugin"), NAME_None, FdungeontooltestStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};