// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Engine_Ass1 : ModuleRules
{
	public Engine_Ass1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
