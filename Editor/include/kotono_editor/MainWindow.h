#pragma once
#include "generated/MainWindow.generated.h"
#include <kotono_interface/Widget.h>
class WMainWindow : public WWidget
{
	GENERATED_WMAINWINDOW()

public:
	WidgetPtr Build() override;
};

