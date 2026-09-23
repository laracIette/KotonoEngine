#include "ValueBox.h"

#include "InputTextBox.h"
#include <kotono_interface/widgets.h>

WidgetPtr WValueBox::Build()
{
	return (
		UCreate<WWrap>{}()
		| Apply(&WWrap::SetAxis, EAxis::Vertical)
		| (
			UCreate<WInputTextBox>{}()
			| Apply(&WInputTextBox::SetText, valueToString_ ? valueToString_() : "")
			| Apply(&WInputTextBox::SetOnTextChanged, stringToValue_)
		)
	);
}

#include "generated/ValueBox.generated.inl"
