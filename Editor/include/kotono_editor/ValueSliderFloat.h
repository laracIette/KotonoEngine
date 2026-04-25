#pragma once
#include "generated/ValueSliderFloat.generated.h"
#include "ValueSlider.h"
class WValueSliderFloat : public WValueSlider<f32>
{
	GENERATED_WVALUESLIDERFLOAT()

public:
	WidgetPtr Build() override;
};
