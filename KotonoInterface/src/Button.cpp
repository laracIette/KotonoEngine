#include "Button.h"
#include <kotono_framework/Framework.h>
#include <kotono_framework/InputManager.h>
#include "log.h"

WButton::WButton(const ButtonSettings& buttonSettings) :
	buttonSettings_(buttonSettings)
{

	Framework.InputManager().Mouse().ButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED)
		.AddListener(KtDelegate(this, &WButton::OnMouseLeftButtonPressed));
	Framework.InputManager().Mouse().ButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN)
		.AddListener(KtDelegate(this, &WButton::OnMouseLeftButtonDown));
	Framework.InputManager().Mouse().ButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_RELEASED)
		.AddListener(KtDelegate(this, &WButton::OnMouseLeftButtonReleased));
	Framework.InputManager().Mouse().GetEventVerticalScroll()
		.AddListener(KtDelegate(this, &WButton::OnMouseVerticalScroll));
}

void WButton::Cleanup()
{
	Framework.InputManager().Mouse().ButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_PRESSED)
		.RemoveListener(KtDelegate(this, &WButton::OnMouseLeftButtonPressed));
	Framework.InputManager().Mouse().ButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_DOWN)
		.RemoveListener(KtDelegate(this, &WButton::OnMouseLeftButtonDown));
	Framework.InputManager().Mouse().ButtonEvent(KT_BUTTON_LEFT, KT_INPUT_STATE_RELEASED)
		.RemoveListener(KtDelegate(this, &WButton::OnMouseLeftButtonReleased));
	Framework.InputManager().Mouse().GetEventVerticalScroll()
		.RemoveListener(KtDelegate(this, &WButton::OnMouseVerticalScroll));

	WWidget::Cleanup();
}

bool WButton::IsMouseHovering() const
{
	const auto& cursorPos{ Framework.InputManager().Mouse().GetCursorPosition() };
	const auto position{ GetPosition() };
	const auto size{ GetSize() };

	return cursorPos.x >= position.x 
		&& cursorPos.x <= position.x + size.x 
		&& cursorPos.y >= position.y 
		&& cursorPos.y <= position.y + size.y;
}

void WButton::OnMouseLeftButtonPressed()
{
	if (!IsMouseHovering())
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

void WButton::OnMouseVerticalScroll(const float delta)
{
	if (!IsMouseHovering())
	{
		return;
	}

	if (buttonSettings_.onVerticalScroll)
	{
		buttonSettings_.onVerticalScroll(delta);
	}
}