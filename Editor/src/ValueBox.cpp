#include "ValueBox.h"
#include <kotono_interface/widgets.h>
#include <InputTextBox.h>

WidgetPtr WValueBox::Build()
{
	UPtr inputTextBox{ UCreate<WInputTextBox>{}() };
	inputTextBox->SetText(valueToString_ ? valueToString_() : "");
	inputTextBox->SetOnTextChanged(stringToValue_);

	UPtr wrap{ UCreate<WWrap>{}() };
	wrap->SetAxis(EAxis::Vertical);

	UChildOwnerTree(
		wrap, new UWidgetTreeLeaf(inputTextBox)
	).Link();

	return wrap;
}

#include "generated/ValueBox.generated.inl"
