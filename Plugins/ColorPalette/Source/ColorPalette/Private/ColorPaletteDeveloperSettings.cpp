// Copyright Omeda Studios, Inc. All Rights Reserved.

#include "ColorPaletteDeveloperSettings.h"

#include "AssetToolsModule.h"
#include "EditorAssetLibrary.h"
#include "ISourceControlModule.h"
#include "SourceControlHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Materials/MaterialParameterCollection.h"
#include "Misc/FileHelper.h"
#include "UObject/PropertyAccessUtil.h"

// Helper functions
void ShowWarningDialog(FString Text)
{
	FText DialogText = FText::FromString(Text);
	FText DialogTitle = FText::FromString(TEXT("Jad Says : "));

	// Open the dialog box with Ok and Cancel buttons
	EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, DialogText, &DialogTitle);
}
FString LinearColorToHSVtring(FLinearColor Color)
{
	return Color.LinearRGBToHSV().ToString();
}
FString GetEnumDisplayName(EColorPaletteColors EnumValue)
{
	const UEnum* Enum = StaticEnum<EColorPaletteColors>();
	if (!Enum)
	{
		return "Invalid";
	}

	return Enum->GetDisplayNameTextByValue(static_cast<int64>(EnumValue)).ToString();
}

// Core
void UColorPaletteDeveloperSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FProperty* Prop = PropertyChangedEvent.Property;
	
	if (Prop && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UColorPaletteDeveloperSettings, Refresh))
	{
		//HandlePropertyChange();
		const UColorPaletteDeveloperSettings* Settings = GetDefault<UColorPaletteDeveloperSettings>();
		if (const TSoftObjectPtr<UMaterialParameterCollection> MPC = Settings->ColorPaletteColorsMPC)
		{
			AddOrSetVecToMPC(MPC.Get());
			SaveColorThemesToIni(GEditorPerProjectIni);
			if(SaveInDefaultIni)
			{
				FString DefaultEditorPerProjectUserSettingsPath = FPaths::Combine(FPaths::ProjectConfigDir(), TEXT("DefaultEditorPerProjectUserSettings.ini"));
				SaveColorThemesToIni(DefaultEditorPerProjectUserSettingsPath);
			}
		}
		else
		{
			ShowWarningDialog("Please make sure there's a valid Color Palette MPC");
		}
	}
	Refresh = false;
}

TArray<FAColorPaletteColor> UColorPaletteDeveloperSettings::GetColorPaletteColors()
{
	TArray<FAColorPaletteColor> Colors;
	for (int32 Index = 0; Index < ColorPaletteColors.Num(); ++Index)
	{
		FColorPaletteColors& CurrentColorPalette = ColorPaletteColors[Index];
		Colors.Append(CurrentColorPalette.Colors);
	}
	return Colors;
}

void ProcessColorArrays(TArray<FAColorPaletteColor>& ColorsPalette, TArray<FCollectionVectorParameter>& ColorsMPC)
{
	// Convert ColorsMPC ParameterNames to a map for quick lookup
	TMap<FString, FCollectionVectorParameter*> ColorsMPCMap;
	for (FCollectionVectorParameter& ColorMPC : ColorsMPC)
	{
		ColorsMPCMap.Add(ColorMPC.ParameterName.ToString(), &ColorMPC);
	}

	for (const FAColorPaletteColor& PaletteColor : ColorsPalette)
	{
		FString KeyAsString = GetEnumDisplayName(PaletteColor.ColorKey);
		FCollectionVectorParameter** FoundColorMPC = ColorsMPCMap.Find(KeyAsString);

		// Check if the key from ColorsPalette is NOT in ColorsMPC
		if (FoundColorMPC == nullptr)
		{
			// Add the new item to ColorsMPC with the color from ColorsPalette
			FCollectionVectorParameter NewColorMPC;
			NewColorMPC.ParameterName = FName(*GetEnumDisplayName(PaletteColor.ColorKey)); // Convert FString to FName
			NewColorMPC.DefaultValue = PaletteColor.Color;        // Set the color from ColorsPalette
			ColorsMPC.Add(NewColorMPC);
		}
		else
		{
			// Update the DefaultValue of the existing item in ColorsMPC
			(*FoundColorMPC)->DefaultValue = PaletteColor.Color;
		}
	}
}

void UColorPaletteDeveloperSettings::AddOrSetVecToMPC(UMaterialParameterCollection* Collection)
{
	// Load the Material Parameter Collection asset
	if (Collection == nullptr)
	{
		ShowWarningDialog("Please make sure there's a valid Color Palette MPC");
		return;
	}

	// Make sure we are able to edit the asset
	if (!Collection->GetOutermost()->IsDirty())
	{
		Collection->Modify();
	}

	TArray<FAColorPaletteColor> Colors = GetColorPaletteColors();
	TArray<FCollectionVectorParameter>& ColorsMPC = Collection->VectorParameters;
	ProcessColorArrays(Colors, ColorsMPC);

	// Mark the collection as modified and save the asset
	Collection->PostEditChange();
	FAssetRegistryModule::AssetCreated(Collection);
	GEditor->BroadcastObjectReimported(Collection);
	
	bool bSavedSuccessfully = UEditorAssetLibrary::SaveAsset(Collection->GetPathName(), false);
	if (!bSavedSuccessfully)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save the asset."));
	}
};

void UColorPaletteDeveloperSettings::SaveColorThemesToIni(FString IniToSaveTo)
{
	if (FPaths::FileExists(IniToSaveTo))
	{
		if(ISourceControlModule::Get().IsEnabled())
		{
			USourceControlHelpers::CheckOutOrAddFile(IniToSaveTo);
		}
		GConfig->EmptySection(TEXT("ColorThemes"), IniToSaveTo);
		for (int32 ThemeIndex = 0; ThemeIndex < ColorPaletteColors.Num(); ++ThemeIndex)
		{
			const FColorPaletteColors& Theme = ColorPaletteColors[ThemeIndex];
			GConfig->SetString(TEXT("ColorThemes"), *FString::Printf(TEXT("Theme%i"), ThemeIndex), *Theme.ColorGroup, IniToSaveTo);

			const TArray<FAColorPaletteColor>& Colors = Theme.Colors;
			for (int32 ColorIndex = 0; ColorIndex < Colors.Num(); ++ColorIndex)
			{
				const FLinearColor& Color = Colors[ColorIndex].Color;
				GetEnumDisplayName(Colors[ColorIndex].ColorKey);
				const FString& Label = GetEnumDisplayName(Colors[ColorIndex].ColorKey);
				GConfig->SetString(TEXT("ColorThemes"), *FString::Printf(TEXT("Theme%iColor%i"), ThemeIndex, ColorIndex), *LinearColorToHSVtring(Color), IniToSaveTo);
				GConfig->SetString(TEXT("ColorThemes"), *FString::Printf(TEXT("Theme%iLabel%i"), ThemeIndex, ColorIndex), *Label, IniToSaveTo);
			}
		}
	}
	else
	{
		FFileHelper::SaveStringToFile(FString("[ColorThemes]"), *IniToSaveTo);
		const FString WarningText = "We couldn't find a" + IniToSaveTo +
									"ini file, we created one for you though, feel free to refresh the operation !";
		ShowWarningDialog(WarningText);
	}
}