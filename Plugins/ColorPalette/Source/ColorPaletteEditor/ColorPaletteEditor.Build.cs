// Copyright (c) 2022 Ryan DowlingSoka

using UnrealBuildTool;

public class ColorPaletteEditor : ModuleRules
{
	public ColorPaletteEditor(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
		);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
		);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"AssetRegistry", 
				"ContentBrowser",
				"CoreUObject",
				"EditorStyle",
				"Engine",
				"Projects",
				"ColorPalette",
				"Slate",
				"SlateCore",
				"UnrealEd",
				"UMGEditor",
				"UMG",
				// ... add private dependencies that you statically link with here ...	
			}
		);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
	}
}