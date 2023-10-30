// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBlueprintEditor.h"
#include "Components/Image.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
DECLARE_DELEGATE(FMyCustomDelegate);

class NINESLICER_API SNineSlicerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNineSlicerWindow)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, TSharedPtr<FWidgetBlueprintEditor>);
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	virtual ~SNineSlicerWindow();
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	void CheckForUpdate();
	void DrawWidget(bool IsSuccess);
	void CheckForUpdateDelegate();

	static void Dothis();
	
	TSharedRef<SWidget> CreateHandle(SNineSlicerWindow* Instance,
									 FVector2D LineSize, FVector2D ButtonOffset,
									 EHorizontalAlignment HAlign_Button, EVerticalAlignment VAlign_Button,
									 EHorizontalAlignment HAlign_Line, EVerticalAlignment VAlign_Line,
									 const FText& ButtonLabel) const;

	EVisibility GetImageVisibility() const;
	EVisibility GetTextVisibility() const;

private:
	TWeakPtr<FWidgetBlueprintEditor> WeakBlueprintEditor;
	FSlateBrush CurrentSlateBrush;
	
	UImage* CurrentImage = nullptr;
	UImage* PreviousImage = CurrentImage;

	UObject* CurrentBrushResource = nullptr;
	UObject* PreviousBrushResource = CurrentBrushResource;

	FText Tip = FText::FromString(TEXT("Select a valid resource \n then hover back to this window."));

private:
	FMyCustomDelegate OnCustomEvent;
};
