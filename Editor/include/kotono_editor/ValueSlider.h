#pragma once
#include "generated/ValueSlider.generated.h"
#include "ValueBox.h"
class WValueSlider : public WValueBox
{
	GENERATED_WVALUESLIDER()

public:
	void Display(UWidgetDisplaySettings displaySettings) override;
	void Remove() override;

protected:
	virtual std::string Clamp(const std::string& value) const;
	virtual std::string Increment(const std::string& value) const;
	virtual std::string Decrement(const std::string& value) const;
};