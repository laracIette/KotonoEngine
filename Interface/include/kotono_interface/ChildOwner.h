#pragma once
#include "generated/ChildOwner.generated.h"
#include <kotono_core/Widget.h>
class WChildOwner : public WWidget
{
	GENERATED_WCHILDOWNER()

public:
	~WChildOwner() override;

public:
	void Remove() override;

	glm::vec2 GetContentSize(glm::vec2 bounds) const override;
	glm::vec2 GetDesiredSize(glm::vec2 const& bounds) const override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const final;
	void PopulateFocusTree(WidgetSet& widgets, glm::vec2 const& cursorPosition) const final;

	b8 OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position) final;
	b8 OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) final;

public:
	void SetChild(const WidgetPtr& widget);

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	ReadonlyProperty(WidgetPtr, child_, Child);
};

class UChildOwnerTree final : public UWidgetTree
{
public:
	UChildOwnerTree(UPtr<WChildOwner> const& widget, UWidgetTree* child);
	~UChildOwnerTree() override;

	WidgetPtr Widget() const override;
	void Link() const override;

private:
	UPtr<WChildOwner> widget_;
	UWidgetTree* child_;
};
