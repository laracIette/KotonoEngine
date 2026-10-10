#pragma once
#include <Widget/Widget.h>

#include <concepts>

#include "ChildOwner.generated.h"

ABSTRACT class WChildOwner : public WWidget
{
	GENERATED()

public:
	WChildOwner();
	~WChildOwner() override;

public:
	void Remove() override;

	void Disown(WidgetPtr const& widget) override;

	auto GetContentSize(glm::vec2 const& bounds) const -> glm::vec2 override;
	auto GetDesiredSize(glm::vec2 const& bounds) const -> glm::vec2 override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const final;
	void PopulateFocusTree(WidgetSet& widgets, glm::vec2 const& cursorPosition) const final;

	auto OnMouseButton(EButton button, EInputState inputState, EModifier modifier) -> b8 override;
	auto OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) -> b8 override;
	auto OnMouseScroll(glm::vec2 const& delta) -> b8 override;

	auto OnKeyboardKey(EKey key, EInputState inputState, EModifier modifier) -> b8 override;

	void Refresh() final;
	
	void CacheBuild() final;

	void SetChild(WidgetPtr const& widget);

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	ReadonlyProperty(WidgetPtr, child_, Child, Value);
};

template <typename T>
concept ChildOwner = requires(T& widget, WidgetPtr const& child)
{
	{ widget.GetChild() } -> std::convertible_to<WidgetPtr>;
	widget.SetChild(child);
};

template <ChildOwner TOwner, std::derived_from<WWidget> TChild>
UPtr<TOwner> const& operator|(UPtr<TOwner> const& owner, UPtr<TChild> const& child)
{
	owner->SetChild(child);
	return owner;
}
