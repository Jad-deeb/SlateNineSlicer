// Copyright Epic Games, Inc. All Rights Reserved.

#include "NineSlicer.h"
#include "NineSlicerStyle.h"
#include "NineSlicerCommands.h"
#include "LevelEditor.h"
#include "NineSlicerTabSummoner.h"
#include "SNineSlicerWindow.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "UMGEditorModule.h"
#include "BlueprintModes/WidgetBlueprintApplicationModes.h"


static const FName NineSlicerTabName("NineSlicer");

#define LOCTEXT_NAMESPACE "FNineSlicerModule"

void FNineSlicerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FNineSlicerStyle::Initialize();
	FNineSlicerStyle::ReloadTextures();

	FNineSlicerCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FNineSlicerCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FNineSlicerModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FNineSlicerModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(NineSlicerTabName, FOnSpawnTab::CreateRaw(this, &FNineSlicerModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FNineSlicerTabTitle", "NineSlicer"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	IUMGEditorModule& UMGEditorModule = FModuleManager::LoadModuleChecked<IUMGEditorModule>("UMGEditor");
	UMGEditorModule.OnRegisterTabsForEditor().AddRaw(this, &FNineSlicerModule::HandleRegisterBlueprintEditorTab);
}

void FNineSlicerModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FNineSlicerStyle::Shutdown();

	FNineSlicerCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(NineSlicerTabName);
}

TSharedRef<SDockTab> FNineSlicerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
	.TabRole(ETabRole::NomadTab)
	[
		SNew(SButton)
	];
}

void FNineSlicerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(NineSlicerTabName);
}

void FNineSlicerModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FNineSlicerCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FNineSlicerCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

void FNineSlicerModule::HandleRegisterBlueprintEditorTab(const FWidgetBlueprintApplicationMode& ApplicationMode, FWorkflowAllowedTabSet& TabFactories)
{
	if (ApplicationMode.GetModeName() == FWidgetBlueprintApplicationModes::DesignerMode)
	{
		TabFactories.RegisterFactory(MakeShared<FNineSlicerTabSummoner>(ApplicationMode.GetBlueprintEditor()));
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNineSlicerModule, NineSlicer)