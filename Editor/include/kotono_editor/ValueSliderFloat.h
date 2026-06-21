#pragma once
#include "generated/ValueSliderFloat.generated.h"
#include "ValueSlider.h"
class WValueSliderFloat : public WValueSlider
{
	GENERATED_WVALUESLIDERFLOAT()
		
public:
	WValueSliderFloat();

protected:
	std::string Clamp(const std::string& value) const final;
	std::string Increment(const std::string& value) const final;
	std::string Decrement(const std::string& value) const final;

private:
	WritableProperty(f32, defaultValue_, DefaultValue);
	WritableProperty(f32, minValue_, MinValue);
	WritableProperty(f32, maxValue_, MaxValue);
	WritableProperty(f32, step_, Step);
};
