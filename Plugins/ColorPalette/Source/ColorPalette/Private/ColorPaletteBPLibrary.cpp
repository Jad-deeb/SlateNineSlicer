// Copyright Epic Games, Inc. All Rights Reserved.

#include "ColorPaletteBPLibrary.h"
#include "ColorPalette.h"
#include "ColorPaletteColorTypes.h"
#include "ColorPaletteDeveloperSettings.h"

UColorPaletteBPLibrary::UColorPaletteBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

TMap<EColorPaletteColors, FLinearColor> GetMyColorPaletteColorsAsTMap(TArray<FColorPaletteColors> ColorPaletteColors)
{
	TMap<EColorPaletteColors, FLinearColor> ColorMap;
	TArray<FAColorPaletteColor> Colors;

	for (int32 Index = 0; Index < ColorPaletteColors.Num(); ++Index)
	{
		FColorPaletteColors CurrentColorPalette = ColorPaletteColors[Index];
		Colors.Append(CurrentColorPalette.Colors);
	}
	for (FAColorPaletteColor PaletteColor : Colors)
	{
		ColorMap.Add(PaletteColor.ColorKey, PaletteColor.Color);
	}
	return ColorMap;
}

FLinearColor UColorPaletteBPLibrary::GetAColorPaletteColor(EColorPaletteColors ColorKey)
{
	const UColorPaletteDeveloperSettings* Settings = GetDefault<UColorPaletteDeveloperSettings>();
	if(Settings)
	{
		TMap<EColorPaletteColors, FLinearColor> ColorMap = GetMyColorPaletteColorsAsTMap(Settings->ColorPaletteColors);
		return {ColorMap[ColorKey]};
	}
	return FLinearColor(0,0,0,0);
}

