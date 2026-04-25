#pragma once
#include "generated/ValueBox.generated.h"
#include <kotono_interface/Widget.h>
class WValueBox : public WWidget
{
	GENERATED_WVALUEBOX()

public:
	using ValueToStringFunction = std::function<std::string()>;
	using StringToValueFunction = std::function<void(const std::string&)>;

public:
	WidgetPtr Build() override;

private:
	WritableProperty(ValueToStringFunction, valueToString_, ValueToString);
	WritableProperty(StringToValueFunction, stringToValue_, StringToValue);
};