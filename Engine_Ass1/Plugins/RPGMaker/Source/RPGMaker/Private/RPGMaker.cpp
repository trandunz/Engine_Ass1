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
			SNew(SRPGMakerMenu)
		];
}

void FRPGMakerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(RPGMakerTabName);
}

void FRPGMakerModule::RegisterMenus()
{
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