// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGMakerCommands.h"

#define LOCTEXT_NAMESPACE "FRPGMakerModule"

void FRPGMakerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "RPGMaker", "Bring up RPGMaker window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
