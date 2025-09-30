#include "Button.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/InputManager.h>
#include "log.h"

WButton::WButton(const ButtonSettings& buttonSettings) :
	buttonSettings_(buttonSettings)
{

	Framework.InputManager().GetMouse().GetEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED)
		.AddListener(KtDelegate(this, &WButton::OnMouseLeftButtonPressed));
	Framework.InputManager().GetMouse().GetEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN)
		.AddListener(KtDelegate(this, &WButton::OnMouseLeftButtonDown));
	Framework.InputManager().GetMouse().GetEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_RELEASED)
		.AddListener(KtDelegate(this, &WButton::OnMouseLeftButtonReleased));
}

void WButton::Cleanup()
{
	Framework.InputManager().GetMouse().GetEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED)
		.RemoveListener(KtDelegate(this, &WButton::OnMouseLeftButtonPressed));
	Framework.InputManager().GetMouse().GetEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN)
		.RemoveListener(KtDelegate(this, &WButton::OnMouseLeftButtonDown));
	Framework.InputManager().GetMouse().GetEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_RELEASED)
		.RemoveListener(KtDelegate(this, &WButton::OnMouseLeftButtonReleased));

	WWidget::Cleanup();
}

void WButton::OnMouseLeftButtonPressed()
{
	const auto& cursorPos{ Framework.InputManager().GetMouse().GetCursorPosition() };
	const auto position{ GetPosition() };
	const auto size{ GetSize() };

	if (cursorPos.x < position.x || cursorPos.x > position.x + size.x ||
		cursorPos.y < position.y || cursorPos.y > position.y + size.y)
	{
		return;
	}

	isPressed_ = true;

	if (buttonSettings_.onPress)
	{
		buttonSettings_.onPress();
	}
}

void WButton::OnMouseLeftButtonDown()
{
	if (!isPressed_)
	{
		return;
	}

	if (buttonSettings_.onDown)
	{
		buttonSettings_.onDown();
	}
}

void WButton::OnMouseLeftButtonReleased()
{
	if (!isPressed_)
	{
		return;
	}

	isPressed_ = false;

	if (buttonSettings_.onReleased)
	{
		buttonSettings_.onReleased();
	}
}
