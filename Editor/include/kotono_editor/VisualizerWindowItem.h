#pragma once
#include "generated/VisualizerWindowItem.generated.h"
#include <kotono_core/Widget.h>
#include "VisualizationField.h"
class WVisualizerWindowItem : public WWidget
{
	GENERATED_WVISUALIZERWINDOWITEM()

public:
	WVisualizerWindowItem(const EVisualizationField field, const std::string& name);

protected:
	WidgetPtr Build() override;

private:
	EVisualizationField field_;
	std::string name_;
};