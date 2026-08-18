#pragma once
#include "generated/ChildOwner.generated.h"
#include <kotono_object/Widget.h>
class WChildOwner : public WWidget
{
	GENERATED_WCHILDOWNER()

public:
	~WChildOwner() override;

public:
	void Remove() override;

	glm::vec2 GetDesiredSize(const glm::vec2& bounds) const override;

	WidgetVector WidgetTree() const override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const override;

public:
	void SetChild(const WidgetPtr& widget);

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) final;

private:
	ReadonlyProperty(WidgetPtr, child_, Child);
};

class UChildOwnerTree final : public UWidgetTree
{
public:
	UChildOwnerTree(const UPtr<WChildOwner>& widget, UWidgetTree* child);
	~UChildOwnerTree() override;

	WidgetPtr Widget() const override;
	void Link() const override;

private:
	UPtr<WChildOwner> widget_;
	UWidgetTree* child_;
};
