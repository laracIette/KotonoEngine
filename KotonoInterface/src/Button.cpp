#include "Button.h"
#include <kotono_framework/Framework.h>
#include "log.h"

WButton::WButton(const ButtonSettings& buttonSettings) :
	buttonSettings_(buttonSettings)
{
	Framework.GetInputManager().GetMouse().GetEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED)
		.AddListener(KtDelegate(this, &WButton::OnMouseLeftButtonPressed));
}

void WButton::Cleanup()
{
	Framework.GetInputManager().GetMouse().GetEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED)
		.RemoveListener(KtDelegate(this, &WButton::OnMouseLeftButtonPressed));

	WWidget::Cleanup();
}

void WButton::OnMouseLeftButtonPressed()
{
	const auto& cursorPos{ Framework.GetInputManager().GetMouse().GetCursorPosition() };
	const auto position{ GetPosition() };
	const auto size{ GetSize() };

	if (cursorPos.x < position.x || cursorPos.x > position.x + size.x ||
		cursorPos.y < position.y || cursorPos.y > position.y + size.y)
	{
		return;
	}

	if (buttonSettings_.onPress)
	{
		buttonSettings_.onPress();
	}
}
