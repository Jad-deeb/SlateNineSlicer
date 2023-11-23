#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "ColorPaletteNode.generated.h"

UCLASS()
class COLORPALETTEEDITOR_API UColorPaletteNode : public UK2Node
{
	GENERATED_BODY()
    
public:
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;

	virtual FLinearColor GetNodeBodyTintColor() const override;
	
	virtual FLinearColor GetNodeTitleColor() const override;

	virtual void AllocateDefaultPins() override;

	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	virtual FText GetTooltipText() const override;

	virtual FText GetToolTipHeading() const override;
	
	virtual bool IsNodePure() const override;
	UEnum* GetEnum() const;

	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

public:
	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;

	FLinearColor MyOutColor = FLinearColor(0.896269,0.637597,0.001518,1);
};