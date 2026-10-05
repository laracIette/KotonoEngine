#pragma once
#include <InterfaceRoot/InterfaceRoot.h>

#include "MainWindow.generated.h"

class WMainWindow final : public WInterfaceRoot
{
	GENERATED()

protected:
	WidgetPtr Build() override;
};
