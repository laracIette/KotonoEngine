#pragma once
#include <kotono_interface\Widget.h>
class WPropertiesWindow : public WWidget
{
	using ValueChangedFunction = std::function<void(float)>;

public:
	WWidget* Build() override;
	void Cleanup() override;

private:
	WWidget* Slider(const std::string& label, const ValueChangedFunction& function);
};

