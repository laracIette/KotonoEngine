#include "ValueSliderFloat.h"
#include <kotono_interface/widgets.h>

WidgetPtr WValueSliderFloat::Build()
{
	return new WBox({
		.size = { 300.0f, 60.0f },
		.child = new WStack({
			.children = {
				new WColor({ Colors::White.WithValue(0.75f) }),
				new WText({
					.text = std::to_string(GetValue()),
					.fontSize = { 42.0f, 50.0f },
					.spacing = -10.0f,
				}),
			},
		}),
	});
}