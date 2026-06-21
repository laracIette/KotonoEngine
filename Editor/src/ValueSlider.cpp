#include "ValueSlider.h"

void WValueSlider::Display(UWidgetDisplaySettings displaySettings)
{
	Base::Display(displaySettings);
}

void WValueSlider::Remove()
{
	Base::Remove();
}

std::string WValueSlider::Clamp(const std::string& value) const
{
	return value;
}

std::string WValueSlider::Increment(const std::string& value) const
{
	return value;
}

std::string WValueSlider::Decrement(const std::string& value) const
{
	return value;
}

#include "generated/ValueSlider.generated.inl"