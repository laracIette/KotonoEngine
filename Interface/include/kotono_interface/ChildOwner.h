#pragma once
#include "generated/ChildOwner.generated.h"
#include <kotono_core/Widget.h>

#include <concepts>
#include <tuple>

class WChildOwner : public WWidget
{
	GENERATED_WCHILDOWNER()

public:
	~WChildOwner() override;

public:
	void Remove() override;

	glm::vec2 GetContentSize(glm::vec2 const& bounds) const override;
	glm::vec2 GetDesiredSize(glm::vec2 const& bounds) const override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph) const final;
	void PopulateFocusTree(WidgetSet& widgets, glm::vec2 const& cursorPosition) const final;

	b8 OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position) override;
	b8 OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position) override;
	b8 OnMouseScroll(glm::vec2 const& delta) override;

	b8 OnKeyboardKey(EKey key, EInputState inputState) override;

	void Refresh() final;

	auto GetChild() const -> WidgetPtr const& { return child_; }
	void SetChild(WidgetPtr const& widget);

protected:
	void DisplayInternal(UWidgetDisplaySettings displaySettings) override;

private:
	WidgetPtr child_;
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
