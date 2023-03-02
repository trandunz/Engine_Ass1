// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGMaker.h"
#include "RPGMakerStyle.h"
#include "RPGMakerCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "RPGMakerMenu/SRPGMakerMenu.h"

static const FName RPGMakerTabName("RPGMaker");

#define LOCTEXT_NAMESPACE "FRPGMakerModule"

void FRPGMakerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FRPGMakerStyle::Initialize();
	FRPGMakerStyle::ReloadTextures();

	FRPGMakerCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FRPGMakerCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FRPGMakerModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FRPGMakerModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(RPGMakerTabName, FOnSpawnTab::CreateRaw(this, &FRPGMakerModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FRPGMakerTabTitle", "RPGMaker"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FRPGMakerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FRPGMakerStyle::Shutdown();

	FRPGMakerCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(RPGMakerTabName);
}

TSharedRef<SDockTab> FRPGMakerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SRPGMakerMenu)
		];
}

void FRPGMakerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(RPGMakerTabName);
}

void FRPGMakerModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FRPGMakerCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FRPGMakerCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRPGMakerModule, RPGMaker)