#pragma once
#include "generated/VisualizerWindow.generated.h"
#include <kotono_object/Widget.h>
#include "VisualizationField.h"
class WVisualizerWindow : public WWidget
{
	GENERATED_WVISUALIZERWINDOW()

protected:
	WidgetPtr Build() override;
};

