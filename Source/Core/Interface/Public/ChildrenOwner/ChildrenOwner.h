#pragma once
#include <Widget/Widget.h>

#include <concepts>

#include "ChildrenOwner.generated.h"

ABSTRACT class WChildrenOwner : public WWidget
{
	GENERATED()

public:
	WChildrenOwner();
	~WChildrenOwner() override;

	void Remove() override;

	void Disown(WidgetPtr const& widget) override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const final;
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

template <typename T>
concept ChildrenOwner = requires(T& widget, WidgetSet const& children, WidgetPtr const& child)
{
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
