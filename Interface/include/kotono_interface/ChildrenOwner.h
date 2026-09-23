#pragma once
#include "generated/ChildrenOwner.generated.h"
#include <kotono_core/Widget.h>

#include <concepts>

class WChildrenOwner : public WWidget
{
	GENERATED_WCHILDRENOWNER()

public:
	~WChildrenOwner() override;

	void Remove() override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const final;
	void PopulateFocusTree(WidgetSet& widgets, glm::vec2 const& cursorPosition) const final;

	b8 OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position) override;
	b8 OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) override;
	b8 OnMouseScroll(glm::vec2 const& delta) override;

	b8 OnKeyboardKey(EKey key, EInputState inputState) override;
	
	void Refresh() final;

	void SetChildren(WidgetSet const& widgets);
	void AddChild(WidgetPtr const& widget);
	void RemoveChild(WidgetPtr const& widget);
	void ReplaceChild(WidgetPtr const& oldWidget, WidgetPtr const& newWidget);

	auto GetChildren() const -> WidgetSet const& { return children_; }

protected:
	size GetValidChildrenCount() const;

private:
	WidgetSet children_;
};

template <typename T>
concept ChildrenOwner = requires(T & widget, WidgetSet const& children, WidgetPtr const& child)
{
	{ widget.GetChildren() } -> std::convertible_to<WidgetSet>;
	widget.SetChildren(children);
	widget.AddChild(child);
};

template <ChildrenOwner TOwner, std::derived_from<WWidget> TChild>
UPtr<TOwner> const& operator|(UPtr<TOwner> const& owner, UPtr<TChild> const& child)
{
	owner->AddChild(child);
	return owner;
}

template <ChildrenOwner TOwner>
UPtr<TOwner> const& operator|(UPtr<TOwner> const& owner, WidgetSet const& children)
{
	owner->SetChildren(children);
	return owner;
}
