// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "RPGMakerStyle.h"

class FRPGMakerCommands : public TCommands<FRPGMakerCommands>
{
public:

	FRPGMakerCommands()
		: TCommands<FRPGMakerCommands>(TEXT("RPGMaker"), NSLOCTEXT("Contexts", "RPGMaker", "RPGMaker Plugin"), NAME_None, FRPGMakerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};