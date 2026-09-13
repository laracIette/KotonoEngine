#pragma once
#include "generated/MainWindow.generated.h"
#include <kotono_core/InterfaceRoot.h>
class WMainWindow final : public WInterfaceRoot
{
	GENERATED_WMAINWINDOW()

protected:
	WidgetPtr Build() override;
};
