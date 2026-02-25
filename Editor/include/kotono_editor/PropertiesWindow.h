#pragma once
#include <kotono_interface\Widget.h>
class WPropertiesWindow : public WWidget
{
	using ValueChangedFunction = std::function<void(float)>;

public:
	WidgetPtr Build() override;
	void Cleanup() override;

private:
	WidgetPtr Slider(const std::string& label, const ValueChangedFunction& function);
};

