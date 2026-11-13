#pragma once
#include <Widget/Widget.h>

#include <concepts>

#include "ChildrenOwner.generated.h"

ABSTRACT class WChildrenOwner : public WWidget
{
	GENERATED()

public:
	~WChildrenOwner() override;

	void Remove() override;

	void Disown(WidgetPtr const& widget) override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph, UColor const& tint) const final;
	void PopulateFocusTree(WidgetSet& widgets, glm::vec2 const& cursorPosition) const final;

	void Refresh() final;
	void CacheBuild() final;

	void SetChildren(WidgetSet const& widgets);
	void AddChild(WidgetPtr const& widget);
	void RemoveChild(WidgetPtr const& widget);
	void ReplaceChild(WidgetPtr const& oldWidget, WidgetPtr const& newWidget);

protected:
	size GetValidChildrenCount() const;

private:
	ReadonlyProperty(WidgetSet, children_, Children);
};

template <class TOwner, class TChild>
concept ChildrenOwner = requires(TOwner& widget, UObjectPtr<TChild> const& child)
{
	widget.AddChild(child);
};

template <std::derived_from<WWidget> TOwner, class TChild>
	requires ChildrenOwner<TOwner, TChild>
auto operator|(UObjectPtr<TOwner> const& owner, UObjectPtr<TChild> const& child) -> UObjectPtr<TOwner> const&
{
	owner->AddChild(child);
	return owner;
}

template <std::derived_from<WWidget> TOwner, std::ranges::input_range R>
	requires ChildrenOwner<TOwner, ptr_type_t<std::ranges::range_value_t<R>>>
auto operator|(UObjectPtr<TOwner> const& owner, R&& children) -> UObjectPtr<TOwner> const&
{
	for (auto const& child : children)
	{
		owner->AddChild(child);
	}
	return owner;
}
