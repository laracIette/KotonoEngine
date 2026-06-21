#include "ValueSliderFloat.h"
#include <kotono_common/conversion_utils.h>

WValueSliderFloat::WValueSliderFloat() 
	: defaultValue_{ 0.0f }
	, minValue_{ -INFINITY }
	, maxValue_{ INFINITY }
	, step_{ 1.0f }
{
}

std::string WValueSliderFloat::Clamp(const std::string& value) const
{
	f32 fValue{ from_string<f32>(value) };
	fValue = std::clamp(fValue, minValue_, maxValue_);
	return std::format("{0}", fValue);
}

std::string WValueSliderFloat::Increment(const std::string& value) const
{
	f32 fValue{ from_string<f32>(value) };
	fValue = std::clamp(fValue + step_, minValue_, maxValue_);
	return std::format("{0}", fValue);
}

std::string WValueSliderFloat::Decrement(const std::string& value) const
{
	f32 fValue{ from_string<f32>(value) };
	fValue = std::clamp(fValue - step_, minValue_, maxValue_);
	return std::format("{0}", fValue);
}

#include "generated/ValueSliderFloat.generated.inl"
