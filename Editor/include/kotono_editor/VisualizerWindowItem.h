#pragma once
#include "generated/VisualizerWindowItem.generated.h"
#include <kotono_core/Widget.h>

#include "VisualizationField.h"

class WVisualizerWindowItem : public WWidget
{
	GENERATED_WVISUALIZERWINDOWITEM()

public:
	WVisualizerWindowItem(EVisualizationField field, std::string_view name);

protected:
	WidgetPtr Build() override;

private:
	EVisualizationField field_;
	std::string name_;
};