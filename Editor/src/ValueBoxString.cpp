#include "ValueBoxString.h"
#include "InputTextBox.h"
#include <kotono_interface/widgets.h>

WWidget* WValueBoxString::Build()
{ 
	return new WWrap({
		.axis = WWrap::Axis::Vertical,
		.child = new WInputTextBox(GetValue()),
	});
}
