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

	void Display(UWidgetDisplaySettings displaySettings) final;
	void Remove() final;

	void Disown(WidgetPtr const& widget) override;

	auto GetContentSize(glm::vec2 const& bounds) const -> glm::vec2 override;
	auto GetDesiredSize(glm::vec2 const& bounds) const -> glm::vec2 override;

	void PopulateRenderGraph(UInterfaceRenderGraph& interfaceRenderGraph, UColor const& tint) const final;
	void PopulateFocusTree(WidgetSet& widgets, glm::vec2 const& cursorPosition) const final;

	void Refresh() final;
	
	void CacheBuild() final;

	void SetChild(WidgetPtr const& widget);
	
protected:
	virtual auto GetChildDisplaySettings(UWidgetDisplaySettings displaySettings) const -> UWidgetDisplaySettings;

private:
	ReadonlyProperty(WidgetPtr, child_, Child, Value);
};

template <typename T>
concept ChildOwner = requires(T& widget, WidgetPtr const& child)
{
	widget.SetChild(child);
};

template <ChildOwner TOwner, std::derived_from<WWidget> TChild>
UObjectPtr<TOwner> const& operator|(UObjectPtr<TOwner> const& owner, UObjectPtr<TChild> const& child)
{
	owner->SetChild(child);
	return owner;
}
