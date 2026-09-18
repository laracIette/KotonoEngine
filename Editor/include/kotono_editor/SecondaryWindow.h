#pragma once
#include "generated/SecondaryWindow.generated.h"
#include <kotono_core/InterfaceRoot.h>
class WSecondaryWindow final : public WInterfaceRoot
{
	GENERATED_WSECONDARYWINDOW()

public:
	WSecondaryWindow(WidgetPtr const& widget);

protected:
	WidgetPtr Build() override;

private:
	WidgetPtr widget_;
};
