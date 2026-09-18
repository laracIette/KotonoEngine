#pragma once
#include "generated/Detachable.generated.h"
#include <kotono_core/Widget.h>
class WColumn;
class WDetachable final : public WWidget
{
	GENERATED_WDETACHABLE()

protected:
	WidgetPtr Build() override;

public:
	auto GetChild() const -> WidgetPtr const& { return child_; }
	void SetChild(WidgetPtr const& widget);

private:
	void Detach();

private:
	WidgetPtr child_;
	UPtr<WColumn> column_;
};