#include "SlateDesigner.h"

#include "ModuleDescriptor.h"
#include "SlateDesignerStyle.h"
#include "SlateDesignerCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Widgets/Input/SCheckBox.h"
#include "PluginDescriptor.h"
#include "Interfaces/IPluginManager.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "GameProjectGenerationModule.h"
#include "PluginBrowser/Private/PluginBrowserModule.h"
#include "ProjectDescriptor.h"
#include "PluginUtils.h"
#include "Interfaces/IMainFrameModule.h"
#include "PluginBrowser/Private/DefaultPluginWizardDefinition.h"
#include "SourceCodeNavigation.h"
#include "PluginBrowser/Private/SNewPluginWizard.h"
#include "SlateDesigner/Menu/SPluginDesignerMenu.h"

struct FPluginTemplateDescription;
static const FName SlateDesignerTabName("SlateDesigner");

#define LOCTEXT_NAMESPACE "FSlateDesignerModule"

void FSlateDesignerModule::StartupModule()
{
	FSlateDesignerStyle::Initialize();
	FSlateDesignerStyle::ReloadTextures();

	FSlateDesignerCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSlateDesignerCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FSlateDesignerModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSlateDesignerModule::RegisterMenus));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SlateDesignerTabName, FOnSpawnTab::CreateRaw(this, &FSlateDesignerModule::OnSpawnPluginTab))
	.SetDisplayName(LOCTEXT("SlateDesignerTitle", "SlateDesigner (Example Window)"))
	.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FSlateDesignerModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSlateDesignerStyle::Shutdown();

	FSlateDesignerCommands::Unregister();
	
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SlateDesignerTabName);
}

void FSlateDesignerModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
	{
		FToolMenuSection& Section = Menu->FindOrAddSection("Programming");
		Section.AddMenuEntryWithCommandList(FSlateDesignerCommands::Get().PluginAction, PluginCommands);
	}
	
	UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
	{
		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
		{
			FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSlateDesignerCommands::Get().OpenPluginWindow));
			Entry.SetCommandList(PluginCommands);
		}
	}
}
void FSlateDesignerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(SlateDesignerTabName);
}

TSharedRef<SDockTab> FSlateDesignerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SPluginDesignerMenu)
		];
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSlateDesignerModule, SlateDesigner)