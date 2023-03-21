// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SlateDesigner : ModuleRules
{
	public SlateDesigner(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"AppFramework",
				"Slate",
				"SlateCore",
				"TranslationEditor",
				"AnimGraph",
				"UnrealEd",
				"KismetWidgets",
				"MainFrame",
				"PropertyEditor",
				"ComponentVisualizers",
				
				"Projects",
				"InputCore",
				"ToolMenus",
				"AssetManagerEditor",
				"EditorStyle",
				"ContentBrowser",
				"ContentBrowserData",
				
				"EditorScriptingUtilities",
				"EditorFramework",
				"EditorWidgets",
				"AIModule",
				"PropertyAccessEditor",
				"GameProjectGeneration",
				"AssetSearch",
				"PluginBrowser",
				"PluginWarden",
				"ContentBrowserAssetDataSource",
				"FileUtilities",
				"PakFile",
				"IESFile",
				"BlankPlugin",
				"PluginUtils",
				"SlateCore",
				"SlateFileDialogs",
				"ApplicationCore",
				"UMG",
				"UMGEditor",
				"EditorWidgets",
				"ContentBrowser"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"AppFramework",
				"Slate",
				"SlateCore",
				"TranslationEditor",
				"AnimGraph",
				"UnrealEd",
				"KismetWidgets",
				"MainFrame",
				"PropertyEditor",
				"ComponentVisualizers",
				
				"Projects",
				"InputCore",
				"ToolMenus",
				"AssetManagerEditor",
				"EditorStyle",
				"ContentBrowser",
				"ContentBrowserData",
				
				"EditorScriptingUtilities",
				"EditorFramework",
				"EditorWidgets",
				"AIModule",
				"PropertyAccessEditor",
				"GameProjectGeneration",
				"AssetSearch",
				"PluginBrowser",
				"PluginWarden",
				"ContentBrowserAssetDataSource",
				"FileUtilities",
				"PakFile",
				"IESFile",
				"BlankPlugin",
				"PluginUtils",
				"SlateCore",
				"SlateFileDialogs",
				"ApplicationCore",
				"UMG",
				"UMGEditor",
				"EditorWidgets",
				"ContentBrowser"
			}
		);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
