#pragma once
#include "generated/List.generated.h"
#include <kotono_core/Widget.h>
class WListBody;
class WScrollable;
/// Defines a vertical container for widgets
class WList final : public WWidget
{
	GENERATED_WLIST()

protected:
	WidgetPtr Build() override;

public:
	auto GetSpacing() const -> f32;
	auto GetChildren() const -> WidgetSet const&;

	void SetSpacing(f32 spacing);
	void SetChildren(WidgetSet const& children);
	void AddChild(WidgetPtr const& child);

private:
	UPtr<WScrollable> scrollable_;
	UPtr<WListBody> body_;
};

