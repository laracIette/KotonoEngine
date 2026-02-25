#pragma once
#include <kotono_interface/Widget.h>
#include "VisualizationField.h"
class WVisualizerWindowItem : public WWidget
{
public:
	WVisualizerWindowItem(const EVisualizationField field, const std::string& name);

	WidgetPtr Build() override;

private:
	EVisualizationField field_;
	std::string name_;
};