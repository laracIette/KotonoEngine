#include "ValueBoxString.h"
#include <kotono_interface/widgets.h>
#include <kotono_input/Keyboard.h>

WWidget* WValueBoxString::Build()
{
	Keyboard.EventKey(EKey::Backspace, EInputState::Pressed)
		.AddListener(UDelegate(this, &WValueBoxString::OnKeyBackspacePressed));

	return new WBox({
		.size = { 300.0f, 26.0f },
		.child = new WStack({
			.children = {
				new WColor({ UColor::White().WithAlpha(0.15f) }),
				new WPadding({
					.padding = WPadding::Padding::All(4.0f),
					.child = new WText({
						.text = GetValue(),
						.fontSize = { 14.0f, 18.0f },
						.spacing = -5.0f,
					}),
				}),
			},
		}),
	});
}

void WValueBoxString::Cleanup()
{
	Keyboard.EventKey(EKey::Backspace, EInputState::Pressed)
		.RemoveListener(UDelegate(this, &WValueBoxString::OnKeyBackspacePressed));

	WWidget::Cleanup();
}

void WValueBoxString::OnKeyBackspacePressed()
{
	SetState([this]() {
		std::string& value{ GetValue() };
		if (!value.empty())
		{
			value.pop_back();
		}
	});
}
