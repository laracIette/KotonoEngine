#pragma once
#include "generated/VisualizerWindowItem.generated.h"
#include <kotono_interface/Widget.h>
#include "VisualizationField.h"
class WVisualizerWindowItem : public WWidget
{
	GENERATED_WVISUALIZERWINDOWITEM()

public:
	WVisualizerWindowItem(const EVisualizationField field, const std::string& name);

	WidgetPtr Build() override;

private:
	EVisualizationField field_;
	std::string name_;
};