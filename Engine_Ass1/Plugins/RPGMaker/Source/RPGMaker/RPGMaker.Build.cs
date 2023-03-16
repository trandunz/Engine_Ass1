// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RPGMaker : ModuleRules
{
	public RPGMaker(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine",
				"CoreUObject",
				"InputCore",
				"LevelEditor",
				"Slate",
				"SlateCore",
				"TranslationEditor",
				"EditorStyle",
				"AssetTools",
				"EditorWidgets",
				"UnrealEd",
				"BlueprintGraph",
				"AnimGraph",
				"ComponentVisualizers",
				"PropertyEditor"
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
				"AIModule"
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
