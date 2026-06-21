#include "ValueBox.h"
#include <kotono_interface/widgets.h>
#include <InputTextBox.h>

WidgetPtr WValueBox::Build()
{
	UPtr inputTextBox{ UCreate<WInputTextBox>{}() };
	inputTextBox->SetText(valueToString_ ? valueToString_() : "");
	inputTextBox->SetOnTextChanged(stringToValue_);

	const auto widgetTree{ UChildOwnerTree{ UCreate<WWrap>{}(EAxis::Vertical),
		new UWidgetTreeLeaf{ inputTextBox }
	} };
	widgetTree.Link();

	return widgetTree.Widget();
}

#include "generated/ValueBox.generated.inl"
