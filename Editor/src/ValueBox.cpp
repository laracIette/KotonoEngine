#include "ValueBox.h"
#include <kotono_interface/widgets.h>
#include <InputTextBox.h>

WValueBox::WValueBox(const ValueBoxSettings& valueBoxSettings) 
	: valueBoxSettings_(valueBoxSettings)
	//, value_(valueBoxSettings.valueToString())
{
}

WWidget* WValueBox::Build()
{
	return new WWrap({
		.axis = WWrap::Axis::Vertical,
		.child = new WInputTextBox({
			.text = valueBoxSettings_.valueToString(),
			.onTextChanged = valueBoxSettings_.stringToValue,
		}),
	});
}