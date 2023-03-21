#pragma once

#include "CoreMinimal.h"
#include "PluginBrowser/Private/NewPluginDescriptorData.h"
#include "PluginBrowser/Private/SFilePathBlock.h"

struct FPluginTemplateDescription;
class IPluginWizardDefinition;
class FToolBarBuilder;
class FMenuBuilder;

class FSlateDesignerModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterMenus();
	void PluginButtonClicked();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	
	TSharedPtr<class FUICommandList> PluginCommands;
};
