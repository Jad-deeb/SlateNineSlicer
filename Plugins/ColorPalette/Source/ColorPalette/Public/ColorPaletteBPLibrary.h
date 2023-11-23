// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ColorPaletteColorTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ColorPaletteBPLibrary.generated.h"

UCLASS()
class COLORPALETTE_API UColorPaletteBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FLinearColor GetAColorPaletteColor(EColorPaletteColors ColorKey);
};
