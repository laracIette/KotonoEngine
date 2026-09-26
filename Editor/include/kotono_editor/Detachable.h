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
	void SetChild(WidgetPtr const& widget);

private:
	void Detach();

private:
	ReadonlyProperty(WidgetPtr, child_, Child, Value);
	UPtr<WColumn> column_;
};