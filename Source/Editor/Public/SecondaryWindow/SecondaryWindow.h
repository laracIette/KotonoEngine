#pragma once
#include <InterfaceRoot/InterfaceRoot.h>

#include "SecondaryWindow.generated.h"

class WSecondaryWindow final : public WInterfaceRoot
{
	GENERATED()

public:
	WSecondaryWindow(WidgetPtr const& widget);

protected:
	WidgetPtr Build() override;

private:
	WidgetPtr widget_;
};
