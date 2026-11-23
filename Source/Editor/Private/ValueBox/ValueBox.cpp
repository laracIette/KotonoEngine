#include "ValueBox.h"

#include "InputTextBox/InputTextBox.h"
#include <core_widgets.h>

auto WValueBox::Build() -> WidgetPtr
{
	return (
		UCreate<WInputTextBox>{}()
		| Apply(&WInputTextBox::SetIsEditable, isEditable_)
		| Apply(&WInputTextBox::SetValueToString, [this]() { return valueToString_ ? valueToString_() : ""; })
		| Apply(&WInputTextBox::SetOnTextChanged, [this](std::string_view text) { if (onTextChanged_) onTextChanged_(text); })
	);
}

#include "ValueBox.generated.inl"
