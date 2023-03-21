// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SlateDesignerStyle.h"

class FSlateDesignerCommands : public TCommands<FSlateDesignerCommands>
{
public:

	FSlateDesignerCommands()
		: TCommands<FSlateDesignerCommands>(TEXT("SlateDesigner"), NSLOCTEXT("Contexts", "SlateDesigner", "SlateDesigner Plugin"), NAME_None, FSlateDesignerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> PluginAction;
	TSharedPtr<FUICommandInfo> OpenPluginWindow;
};
