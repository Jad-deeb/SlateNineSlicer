// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ColorPaletteColorTypes.generated.h"


// Voyager colors
UENUM(BlueprintType)
enum class EColorPaletteColors : uint8
{
	Value1 UMETA(DisplayName = "Main"),
	Value2 UMETA(DisplayName = "Secondary"),
	Value3 UMETA(DisplayName = "Odd"),
	Value4 UMETA(DisplayName = "White"),
	Value5 UMETA(DisplayName = "Unselected"),
	Value6 UMETA(DisplayName = "Unavailable"),
	Value7 UMETA(DisplayName = "Background"),
	Value8 UMETA(DisplayName = "Dark"),
	Value9 UMETA(DisplayName = "Black"),
	Value10 UMETA(DisplayName = "Positive-2"),
	Value11 UMETA(DisplayName = "Positive-1"),
	Value12 UMETA(DisplayName = "Negative-1"),
	Value13 UMETA(DisplayName = "Negative-2"),
	Value14 UMETA(DisplayName = "Legendary"),
	Value15 UMETA(DisplayName = "Epic"),
	Value16 UMETA(DisplayName = "Rare"),
	Value17 UMETA(DisplayName = "Uncommon"),
	Value18 UMETA(DisplayName = "Common"),
	Value19 UMETA(DisplayName = "Master"),
	Value20 UMETA(DisplayName = "Expert"),
	Value21 UMETA(DisplayName = "Proficient"),
	Value22 UMETA(DisplayName = "Adept"),
};

USTRUCT(BlueprintType)
struct FAColorPaletteColor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EColorPaletteColors ColorKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color;

	// Default constructor
	FAColorPaletteColor()
		: ColorKey(EColorPaletteColors::Value1), Color(FLinearColor::White) {} // Default values

	// Custom constructor
	FAColorPaletteColor(EColorPaletteColors InColorKey, FLinearColor InColor)
		: ColorKey(InColorKey), Color(InColor) {}
};

USTRUCT(BlueprintType)
struct FColorPaletteColors
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ColorGroup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAColorPaletteColor> Colors;

	// Default constructor
	FColorPaletteColors()
		: ColorGroup(TEXT("DefaultGroup")) {}

	// Custom constructor
	FColorPaletteColors(FString InColorGroup, TArray<FAColorPaletteColor> InColors)
		: ColorGroup(InColorGroup), Colors(InColors) {}
};

