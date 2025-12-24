#include "Button.h"
#include <kotono_common/log.h>
#include <kotono_input/Mouse.h>

WButton::WButton(const ButtonSettings& buttonSettings) :
	buttonSettings_(buttonSettings)
{

	Mouse.EventButton(EButton::Left, EInputState::Pressed)
		.AddListener(KtDelegate(this, &WButton::OnMouseLeftButtonPressed));
	Mouse.EventButton(EButton::Left, EInputState::Down)
		.AddListener(KtDelegate(this, &WButton::OnMouseLeftButtonDown));
	Mouse.EventButton(EButton::Left, EInputState::Released)
		.AddListener(KtDelegate(this, &WButton::OnMouseLeftButtonReleased));
	Mouse.EventVerticalScroll()
		.AddListener(KtDelegate(this, &WButton::OnMouseVerticalScroll));
}

void WButton::Cleanup()
{
	Mouse.EventButton(EButton::Left, EInputState::Pressed)
		.RemoveListener(KtDelegate(this, &WButton::OnMouseLeftButtonPressed));
	Mouse.EventButton(EButton::Left, EInputState::Down)
		.RemoveListener(KtDelegate(this, &WButton::OnMouseLeftButtonDown));
	Mouse.EventButton(EButton::Left, EInputState::Released)
		.RemoveListener(KtDelegate(this, &WButton::OnMouseLeftButtonReleased));
	Mouse.EventVerticalScroll()
		.RemoveListener(KtDelegate(this, &WButton::OnMouseVerticalScroll));

	WWidget::Cleanup();
}

bool WButton::IsMouseHovering() const
{
	const auto& cursorPos{ Mouse.CursorPosition() };
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