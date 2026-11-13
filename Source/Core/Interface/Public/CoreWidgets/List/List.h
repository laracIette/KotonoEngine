#pragma once
#include "WidgetContainer/WidgetContainer.h"

#include "List.generated.h"

class WListBody;
class WScrollable;

/// Defines a vertical container for widgets
class WList final : public WWidgetContainer
{
	GENERATED()

public:
	WList();
	
protected:
	WidgetPtr Build() override;

public:
	auto GetChildren() const -> WidgetSet const& override;
	void SetChildren(WidgetSet const& children) override;
	void AddChild(WidgetPtr const& child) override;

	auto GetSpacing() const -> f32;
	void SetSpacing(f32 spacing) const;
	void ReplaceChild(WidgetPtr const& oldWidget, WidgetPtr const& newWidget) const;

private:
	UObjectPtr<WScrollable> scrollable_;
	UObjectPtr<WListBody> body_;
};

