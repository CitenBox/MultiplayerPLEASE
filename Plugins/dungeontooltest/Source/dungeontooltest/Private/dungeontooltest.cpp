// Copyright Epic Games, Inc. All Rights Reserved.

#include "dungeontooltest.h"
#include "dungeontooltestStyle.h"
#include "dungeontooltestCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName dungeontooltestTabName("dungeontooltest");

#define LOCTEXT_NAMESPACE "FdungeontooltestModule"

void FdungeontooltestModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FdungeontooltestStyle::Initialize();
	FdungeontooltestStyle::ReloadTextures();

	FdungeontooltestCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FdungeontooltestCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FdungeontooltestModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FdungeontooltestModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(dungeontooltestTabName, FOnSpawnTab::CreateRaw(this, &FdungeontooltestModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FdungeontooltestTabTitle", "dungeontooltest"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FdungeontooltestModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FdungeontooltestStyle::Shutdown();

	FdungeontooltestCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(dungeontooltestTabName);
}

TSharedRef<SDockTab> FdungeontooltestModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FdungeontooltestModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("dungeontooltest.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FdungeontooltestModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(dungeontooltestTabName);
}

void FdungeontooltestModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FdungeontooltestCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FdungeontooltestCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FdungeontooltestModule, dungeontooltest)