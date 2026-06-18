#pragma once
#include "generated/ValueSliderFloat.generated.h"
#include "ValueSlider.h"
class WValueSliderFloat : public WValueSlider<f32>
{
	GENERATED_WVALUESLIDERFLOAT()

protected:
	WidgetPtr Build() override;
};
