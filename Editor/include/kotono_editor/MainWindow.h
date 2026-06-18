#pragma once
#include "generated/MainWindow.generated.h"
#include <kotono_interface/Widget.h>
class WMainWindow : public WWidget
{
	GENERATED_WMAINWINDOW()

public:
	void BeginDraw();
	void EndDraw();

protected:
	WidgetPtr Build() override;
};

