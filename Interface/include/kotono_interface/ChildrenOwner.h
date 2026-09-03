#pragma once
#include "generated/ChildrenOwner.generated.h"
#include <kotono_core/Widget.h>
class WChildrenOwner : public WWidget
{
	GENERATED_WCHILDRENOWNER()

public:
	~WChildrenOwner() override;

	void Remove() override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const final;
	void PopulateFocusTree(WidgetSet& widgets, glm::vec2 const& cursorPosition) const final;

	b8 OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position) final;
	b8 OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) final;

	void SetChildren(WidgetSet const& widgets);

protected:
	size GetValidChildrenCount() const;

private:
	ReadonlyProperty(WidgetSet, children_, Children);
};

class UChildrenOwnerTree final : public UWidgetTree
{
public:
	UChildrenOwnerTree(UPtr<WChildrenOwner> const& widget, std::span<UWidgetTree* const> children);
	UChildrenOwnerTree(UPtr<WChildrenOwner> const& widget, std::initializer_list<UWidgetTree*> children);
	~UChildrenOwnerTree() override;

	WidgetPtr Widget() const override;
	void Link() const override;

private:
	UPtr<WChildrenOwner> widget_;
	std::vector<UWidgetTree*> children_;
};
