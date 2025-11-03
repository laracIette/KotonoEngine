#pragma once
#include <kotono_interface/Widget.h>
#include "VisualizationField.h"
class WVisualizerWindow : public WWidget
{
public:
	WWidget* Build() override;

private:
	WWidget* BuildVisualizationFieldWidget(const EVisualizationField field, const std::string& name);
};

