#include "ValueBox.h"

#include "InputTextBox.h"
#include <kotono_interface/widgets.h>

WidgetPtr WValueBox::Build()
{
	return (
		UCreate<WInputTextBox>{}()
		| Apply(&WInputTextBox::SetValueToString, [this]() { return valueToString_ ? valueToString_() : ""; })
		| Apply(&WInputTextBox::SetOnTextChanged, [this](std::string_view text) { if (onTextChanged_) onTextChanged_(text); })
	);
}

#include "generated/ValueBox.generated.inl"
