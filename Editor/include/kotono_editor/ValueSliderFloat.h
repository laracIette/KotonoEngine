#pragma once
#include "ValueSlider.h"
class WValueSliderFloat : public WValueSlider<f32>
{
	using WValueSlider::WValueSlider;

public:
	WWidget* Build() override;
};