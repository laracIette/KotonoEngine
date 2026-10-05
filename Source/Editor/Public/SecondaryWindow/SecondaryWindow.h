#pragma once
#include "SecondaryWindow.generated.h"
#include <InterfaceRoot/InterfaceRoot.h>

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
