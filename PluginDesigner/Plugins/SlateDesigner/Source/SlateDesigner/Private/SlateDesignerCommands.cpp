// Copyright Epic Games, Inc. All Rights Reserved.

#include "SlateDesignerCommands.h"

#define LOCTEXT_NAMESPACE "FSlateDesignerModule"

void FSlateDesignerCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "SlateDesigner", "Execute SlateDesigner action", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(OpenPluginWindow, "SlateDesigner", "Bring up SlateDesigner window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
