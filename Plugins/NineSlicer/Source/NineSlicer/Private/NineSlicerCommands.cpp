// Copyright Epic Games, Inc. All Rights Reserved.

#include "NineSlicerCommands.h"

#define LOCTEXT_NAMESPACE "FNineSlicerModule"

void FNineSlicerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "NineSlicer", "Bring up NineSlicer window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
