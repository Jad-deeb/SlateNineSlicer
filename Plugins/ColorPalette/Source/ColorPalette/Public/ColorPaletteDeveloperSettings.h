// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ColorPaletteColorTypes.h"
#include "Engine/DeveloperSettings.h"
#include "ColorPaletteDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(config=Game, defaultconfig, meta = (DisplayName = "Color Palette Settings"))
class COLORPALETTE_API UColorPaletteDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public: //Voyager color settings
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = "ColorPalette Colors")
	bool Refresh;

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = "ColorPalette Colors")
	TArray<FColorPaletteColors> ColorPaletteColors;

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = "Development", meta = (DisplayName = "Color Palette MPC"))
	TSoftObjectPtr<UMaterialParameterCollection> ColorPaletteColorsMPC;

	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = "Development", meta = (DisplayName = "Store colors to all users by default ?"))
	bool SaveInDefaultIni = false;

public:
	UFUNCTION()
	void AddOrSetVecToMPC(UMaterialParameterCollection* Collection);

	UFUNCTION()
	TArray<FAColorPaletteColor> GetColorPaletteColors();
	
	UFUNCTION()
	void SaveColorThemesToIni(FString IniToSaveTo);
	
};
