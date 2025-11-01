#pragma once
#include <kotono_interface/Widget.h>
#include <kotono_engine/VisualizationField.h>
class WVisualizerWindow : public WWidget
{
public:
	WWidget* Build() override;

private:
	WWidget* BuildVisualizationFieldWidget(const EVisualizationField field, const std::string& name);
};

