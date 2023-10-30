// Fill out your copyright notice in the Description page of Project Settings.

#include "NineSlicerTabSummoner.h"

#include "NineSlicerStyle.h"
#include "SNineSlicerWindow.h"
#include "StatusBarSubsystem.h"
#include "WidgetBlueprintEditor.h"

#define LOCTEXT_NAMESPACE "TabSummoner"

const FName FNineSlicerTabSummoner::TabID(TEXT("NineSlicerTab"));
const FName FNineSlicerTabSummoner::DrawerID(TEXT("NineSlicerDrawer"));

FNineSlicerTabSummoner::FNineSlicerTabSummoner(TSharedPtr<FWidgetBlueprintEditor> BlueprintEditor, bool bInIsDrawerTab)
	: FWorkflowTabFactory(TabID, BlueprintEditor)
	, WeakWidgetBlueprintEditor(BlueprintEditor)
	, bIsDrawerTab(bInIsDrawerTab)
{
	TabLabel = LOCTEXT("ViewBinding_ViewMenu_Label", "View Binding");
	TabIcon = FSlateIcon(FNineSlicerStyle::Get().GetStyleSetName(), "BlueprintView.TabIcon");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("ViewBinding_ViewMenu_Desc", "View Binding");
	ViewMenuTooltip = LOCTEXT("ViewBinding_ViewMenu_ToolTip", "Show the View Binding tab");

};

TSharedRef<SWidget> FNineSlicerTabSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	return SNew(SNineSlicerWindow, WeakWidgetBlueprintEditor.Pin());
}


void FNineSlicerTabSummoner::ToggleNineSlicerDrawer()
{
	GEditor->GetEditorSubsystem<UStatusBarSubsystem>()->TryToggleDrawer(FNineSlicerTabSummoner::DrawerID);
}
