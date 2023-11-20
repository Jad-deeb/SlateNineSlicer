// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ColorPaletteBPLibrary.generated.h"

UCLASS()
class UColorPaletteBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "ColorPalette sample test testing"), Category = "ColorPaletteTesting")
	static float ColorPaletteSampleFunction(float Param);
};
