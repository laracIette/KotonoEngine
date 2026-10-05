#include "ValueSlider.h"

#include "InputTextBox/InputTextBox.h"
#include <core_widgets.h>

WidgetPtr WValueSlider::Build()
{
	return (
		UCreate<WStack>{}()
		| (
			UCreate<WButton>{}()
			| Apply(&WButton::SetOnDrag, [this](glm::vec2 const& delta) { if (onSlide_) onSlide_(delta.x); })
		)
		| (
			UCreate<WInputTextBox>{}()
			| Apply(&WInputTextBox::SetValueToString, [this]() { return valueToString_ ? valueToString_() : ""; })
			| Apply(&WInputTextBox::SetOnTextChanged, [this](std::string_view text) { if (onTextChanged_) onTextChanged_(text); })
		)
	);
}

#include "generated/ValueSlider.generated.inl"