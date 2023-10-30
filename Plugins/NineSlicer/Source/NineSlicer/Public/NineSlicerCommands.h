// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "NineSlicerStyle.h"

class FNineSlicerCommands : public TCommands<FNineSlicerCommands>
{
public:

	FNineSlicerCommands()
		: TCommands<FNineSlicerCommands>(TEXT("NineSlicer"), NSLOCTEXT("Contexts", "NineSlicer", "NineSlicer Plugin"), NAME_None, FNineSlicerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};