// K2Node_Custom.cpp

#include "ColorPaletteNode.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "ColorPaletteBPLibrary.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "ColorPaletteDeveloperSettings.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "Misc/DefaultValueHelper.h"
#include "EdGraphSchema_K2_Actions.h"
#include "BlueprintEditorSettings.h"
#include "K2Node_CastByteToEnum.h"

/**
 * NOT BEING USED UNTIL I FIGURE OUT K2NODES
 */

#define LOCTEXT_NAMESPACE "ColorPaletteNode"

static const FName EnumPinName = TEXT("EnumPin");
static const FName ColorOutPin = TEXT("ColorOutPin");

void UColorPaletteNode::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FLinearColor UColorPaletteNode::GetNodeBodyTintColor() const
{
	return MyOutColor;
}

FLinearColor UColorPaletteNode::GetNodeTitleColor() const
{
	return MyOutColor;
}

void UColorPaletteNode::AllocateDefaultPins()
{
	UEdGraphPin* BytePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, StaticEnum<EColorPaletteColors>(), EnumPinName);
	if(BytePin)
	{
		BytePin->DefaultValue = TEXT("Value1");
	}
	UEdGraphPin* ColorPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, TBaseStructure<FLinearColor>::Get(), UEdGraphSchema_K2::PN_ReturnValue);
	if (ColorPin)
	{
		ColorPin->PinFriendlyName = FText::FromString("Color");
	}
	Super::AllocateDefaultPins();
}

int32 GetLastIntFromString(const FString& InputString)
{
	FString NumericString;
	for (int32 i = InputString.Len() - 1; i >= 0; --i)
	{
		TCHAR Character = InputString[i];

		if (FChar::IsDigit(Character))
		{
			NumericString.InsertAt(0, Character);
		}
		else if (!NumericString.IsEmpty())
		{
			break;
		}
	}
	int32 NumericValue = 0;
	FDefaultValueHelper::ParseInt(NumericString, NumericValue);

	return NumericValue;
}

TMap<EColorPaletteColors, FLinearColor> GetColorPaletteColorsAsTMap(const UColorPaletteDeveloperSettings* ColorPaletteDeveloperSettings)
{
	TMap<EColorPaletteColors, FLinearColor> ColorMap;
	
	TArray<FAColorPaletteColor> Colors;
	TArray<FColorPaletteColors> ColorPaletteColors = ColorPaletteDeveloperSettings->ColorPaletteColors;
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

void UColorPaletteNode::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	FString InputPinName = Pin->GetDefaultAsString();
	int32 InputPinEnumInt = GetLastIntFromString(InputPinName);
	UE_LOG(LogTemp, Warning, TEXT("text is : %s"), *InputPinName);

	const UColorPaletteDeveloperSettings* ColorPaletteDeveloperSettings = GetDefault<UColorPaletteDeveloperSettings>();
	TMap<EColorPaletteColors, FLinearColor> ColorMap = GetColorPaletteColorsAsTMap(ColorPaletteDeveloperSettings);
	FLinearColor* MyColor = ColorMap.Find(static_cast<EColorPaletteColors>(InputPinEnumInt-1));
	if(MyColor)
	{
		MyOutColor = *MyColor;
	}
	else
	{
		FText DialogText = FText::FromString("Color not defined yet !");
		FText DialogTitle = FText::FromString(TEXT("Jad Says :"));
		EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, DialogText, &DialogTitle);
		Pin->ResetDefaultValue();
		MyOutColor = FLinearColor(1,1,1,1);
	}
	Super::PinDefaultValueChanged(Pin);
	if (UBlueprint* Blueprint = GetBlueprint())
	{
		FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
	}
}

FText UColorPaletteNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Get ColorPalette Color"));
}

FText UColorPaletteNode::GetTooltipText() const
{
	return FText::FromString(TEXT("Returns ColorPalette color instance found in Project Settings > Game > ColorPalette Color Settings"));
}

FText UColorPaletteNode::GetToolTipHeading() const
{
	return FText::FromString(TEXT("Get ColorPalette Color"));
}

bool UColorPaletteNode::IsNodePure() const
{
	return true;
}

UEnum* UColorPaletteNode::GetEnum() const
{
	const UEdGraphPin* InputPin = FindPinChecked(EnumPinName);
	const UEdGraphPin* EnumPin = InputPin->LinkedTo.Num() ? InputPin->LinkedTo[0] : InputPin;
	return EnumPin ? Cast<UEnum>(EnumPin->PinType.PinSubCategoryObject.Get()) : nullptr;
}


FSlateIcon UColorPaletteNode::GetIconAndTint(FLinearColor& OutColor) const
{
	return FSlateIcon("EditorStyle", "ColorPicker.Mode");
}

void UColorPaletteNode::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);
	//
	// UEdGraphPin* InPinSwitch = FindPin(EnumPinName);
	// UEdGraphPin* OutPinCase = FindPin(ColorOutPin);
	//
	// UK2Node_CallFunction* PinCallFunction = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	// const UFunction* Function = UColorPaletteBPLibrary::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UColorPaletteBPLibrary, GetAColorPaletteColor));
	// PinCallFunction->SetFromFunction(Function);
	// PinCallFunction->AllocateDefaultPins();
	//
	// UEdGraphPin *TagContainerPin = PinCallFunction->FindPinChecked(EnumPinName);
	// CompilerContext.CopyPinLinksToIntermediate(*InPinSwitch, *TagContainerPin);
	//
	// UEdGraphPin *OutPin = PinCallFunction->FindPinChecked(UEdGraphSchema_K2::PC_Struct);
	//
	// if (OutPinCase && OutPin)
	// {
	// 	OutPin->PinType = OutPinCase->PinType; // Copy type so it uses the right actor subclass
	// 	CompilerContext.MovePinLinksToIntermediate(*OutPinCase, *OutPin);
	// }
	//
	// BreakAllNodeLinks();
}

// void UColorPaletteNode::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
// {
// 	Super::ExpandNode(CompilerContext, SourceGraph);
//
// 	// Get the function reference
// 	UFunction* GetColorFunction = UColorPaletteBPLibrary::StaticClass()->FindFunctionByName("GetAColorPaletteColor");
// 	if (!GetColorFunction)
// 	{
// 		CompilerContext.MessageLog.Error(*FText(LOCTEXT("InvalidFunction", "Invalid function for node @@")).ToString(), this);
// 		return;
// 	}
//
// 	// Create a new 'call function' node in the graph
// 	UK2Node_CallFunction* CallFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
// 	CallFuncNode->FunctionReference.SetExternalMember("GetAColorPaletteColor", UColorPaletteBPLibrary::StaticClass());
// 	CallFuncNode->AllocateDefaultPins();
//
// 	CompilerContext.MovePinLinksToIntermediate(*FindPin(ColorOutPin), *(CallFuncNode->FindPinChecked(EnumPinName)));
// 	CompilerContext.MovePinLinksToIntermediate(*FindPin(EnumPinName), *(CallFuncNode->GetReturnValuePin()));
//
// 	// Break any links to the expanded node's exec pins
// 	BreakAllNodeLinks();
// }
//
void UColorPaletteNode::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	//UEdGraphPin* InputPin = FindPin(TEXT("ExamplePinName"));
	FString InputPinName = Pin->GetDefaultAsString();
	
	UE_LOG(LogTemp, Warning, TEXT("text is : %s"), *InputPinName);

	if (UBlueprint* Blueprint = GetBlueprint())
	{
		FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
	}
}



#undef LOCTEXT_NAMESPACE
