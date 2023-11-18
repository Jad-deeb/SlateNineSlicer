// Fill out your copyright notice in the Description page of Project Settings.

#include "SNineSlicerWindow.h"
#include "SlateOptMacros.h"
#include "WidgetBlueprint.h"
#include "Components/Image.h"
#include "Widgets/Layout/SScaleBox.h"

void SNineSlicerWindow::Construct(const FArguments& InArgs, TSharedPtr<FWidgetBlueprintEditor> WidgetBlueprintEditor)
{
	CheckForUpdateBind.BindSP(this, &SNineSlicerWindow::CheckForUpdateDelegate);
	WeakBlueprintEditor = WidgetBlueprintEditor;
	CheckForUpdate();
}

int32 SNineSlicerWindow::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if(CheckForUpdateBind.IsBound())
	{
		CheckForUpdateBind.Execute();
	}

	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                                bParentEnabled);
}

SNineSlicerWindow::~SNineSlicerWindow()
{
	if (TSharedPtr<FWidgetBlueprintEditor> WidgetEditor = WeakBlueprintEditor.Pin())
	{
		if (UWidgetBlueprint* WidgetBlueprint = WidgetEditor->GetWidgetBlueprintObj())
		{
			WidgetBlueprint->OnExtensionAdded.RemoveAll(this);
		}
	}
}

bool bShouldShowImage = false;

void SNineSlicerWindow::CheckForUpdate()
{
	TSharedPtr<FWidgetBlueprintEditor> WBPEditor = WeakBlueprintEditor.Pin();
	UWidgetBlueprint* WidgetBlueprint = WBPEditor->GetWidgetBlueprintObj();
	check(WidgetBlueprint);

	TSet<FWidgetReference> Refs = WBPEditor->GetSelectedWidgets();
	for (const FWidgetReference& WidgetRef : Refs)
	{
		if(UImage* Img = Cast<UImage>(WidgetRef.GetPreview()))
		{
			CurrentImage = Img;
			CurrentSlateBrush = Img->GetBrush();
			CurrentBrushResource = CurrentSlateBrush.GetResourceObject();
			if(CurrentImage != PreviousImage || CurrentBrushResource != PreviousBrushResource)
			{
				PreviousImage = CurrentImage;
				PreviousBrushResource = CurrentBrushResource;
				//update filled
				DrawWidget(true);
				
				bShouldShowImage = true;
			}
		}
		else
		{
			// update empty
			SNineSlicerWindow::DrawWidget(false);
			CurrentImage = nullptr;
			PreviousBrushResource = CurrentImage;

			bShouldShowImage = false;
		}
	}
}

EVisibility SNineSlicerWindow::GetImageVisibility() const
{
	return bShouldShowImage ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility SNineSlicerWindow::GetTextVisibility() const
{
	return bShouldShowImage ? EVisibility::Collapsed : EVisibility::Visible;
}

void SNineSlicerWindow::DrawWidget(bool IsSuccess)
{
	ChildSlot
	[
		SNew(SOverlay)

		+SOverlay::Slot()
		[
			SNew(SImage).Image(&CurrentSlateBrush)
			.Visibility(this, &SNineSlicerWindow::GetImageVisibility)
		]

		// Draw the handles

		// Top Handle
		+SOverlay::Slot()
		[
			CreateHandle(this,FVector2D(1,3), FVector2D(0,-8),
					HAlign_Center, VAlign_Top,
					HAlign_Fill, VAlign_Top,
					FText::FromString("T"), T_Transform)
		]

		// Bottom Handle
		+SOverlay::Slot()
		[
			CreateHandle(this,FVector2D(1,3), FVector2D(0,8),
						HAlign_Center, VAlign_Bottom,
						HAlign_Fill, VAlign_Bottom,
						FText::FromString("B"), B_Transform)
		]

		// Left Handle
		+SOverlay::Slot()
		[
			CreateHandle(this,FVector2D(3,1), FVector2D(-8,0),
						HAlign_Left, VAlign_Center,
						HAlign_Left, VAlign_Fill,
						FText::FromString("L"), L_Transform)
		]
		
		// Right Handle
		+SOverlay::Slot()
		[
			CreateHandle(this,FVector2D(3,1), FVector2D(8,0),
						HAlign_Right, VAlign_Center,
						HAlign_Right, VAlign_Fill,
						FText::FromString("R"), R_Transform)
		]

		// If no uimage is selected
		+SOverlay::Slot()
		[
			SNew(STextBlock)
			.Text(Tip)
			.Visibility(this, &SNineSlicerWindow::GetTextVisibility)
		]
	];
}

void SNineSlicerWindow::CheckForUpdateDelegate()
{
	CheckForUpdate();
}

void SNineSlicerWindow::OnPressedLambda(const FText& label)
{
	UE_LOG(LogTemp, Warning, TEXT("Label: %s"), *label.ToString());
	
	if(label.ToString() == "L")
	{
		Allow_L = true;
		Allow_R = Allow_T = Allow_B = false;
		return;
	}
	if(label.ToString() == "R")
	{
		Allow_R = true;
		Allow_L = Allow_T = Allow_B = false;
		return;
	}
	if(label.ToString() == "T")
	{
		Allow_T = true;
		Allow_L = Allow_R = Allow_B = false;
		return;
	}
	if(label.ToString() == "B")
	{
		Allow_B = true;
		Allow_L = Allow_R = Allow_T = false;
		return;
	}
}

void SNineSlicerWindow::OnReleasedLambda()
{
	Allow_L = Allow_R = Allow_T = Allow_B = false;
}

TSharedRef<SWidget> SNineSlicerWindow::CreateHandle(SNineSlicerWindow* Instance,
                                                    FVector2D LineSize,
                                                    FVector2D ButtonOffset, EHorizontalAlignment HAlign_Button, EVerticalAlignment VAlign_Button,
                                                    EHorizontalAlignment HAlign_Line, EVerticalAlignment VAlign_Line, const FText& ButtonLabel, FVector2D Transform) const
{
	
	return SNew(SOverlay)
	+SOverlay::Slot().HAlign(HAlign_Line).VAlign(VAlign_Line)
	[
		SNew(SImage)
		.DesiredSizeOverride(LineSize)
		.ColorAndOpacity(FLinearColor::Blue)
		.RenderTransform(Transform)
	]
	+SOverlay::Slot().HAlign(HAlign_Button).VAlign(VAlign_Button)
	[
		SNew(SBox)
		.RenderTransform(ButtonOffset+Transform)
		[
			SNew(SButton)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.ContentPadding(FMargin(-6,1,-6,1))
			.OnPressed(FSimpleDelegate::CreateLambda([Instance, ButtonLabel]()//absolutely vile lambda
						{
							Instance->OnPressedLambda(ButtonLabel);
						}))
			.OnReleased(Instance, &SNineSlicerWindow::OnReleasedLambda)
			[
				SNew(STextBlock)
				.Text(ButtonLabel)
				.Font(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 8))
			]
		]
	];
}

FReply SNineSlicerWindow::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ayayyaya"));
		// Return FReply::Handled() to indicate that we've handled this event.
		return FReply::Handled();
	}
	UE_LOG(LogTemp, Warning, TEXT("not handling"));
	// If we didn't handle the event, or a different button was pressed, pass the event along.
	return FReply::Unhandled();
}

FReply SNineSlicerWindow::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		FVector2D LocalMousePosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
		LocalMousePosition = LocalMousePosition/MyGeometry.Size;
		UE_LOG(LogTemp, Warning, TEXT("%f %f"), LocalMousePosition.X, LocalMousePosition.Y);
	}
	FVector2D LocalMousePosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	LocalMousePosition = LocalMousePosition/MyGeometry.Size;
	UE_LOG(LogTemp, Warning, TEXT("%f %f"), LocalMousePosition.X, LocalMousePosition.Y);
	//CheckForUpdate();
	//FVector2D LocalMousePosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	//LocalMousePosition = LocalMousePosition/MyGeometry.Size;
	//UE_LOG(LogTemp, Warning, TEXT("%f %f"), LocalMousePosition.X, LocalMousePosition.Y);
	
	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
