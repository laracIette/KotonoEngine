#include "Button.h"
#include "Interface.h"
#include <kotono_common/log.h>
#include <kotono_input/Mouse.h>

WButton::WButton(const ButtonSettings& buttonSettings) 
	: buttonSettings_(buttonSettings)
{
	Interface.AddButton(this);

	Mouse.EventButton(EButton::Left, EInputState::Down)
		.AddListener(UDelegate(this, &WButton::OnMouseLeftButtonDown));
}

void WButton::Cleanup()
{
	Interface.RemoveButton(this);

	Mouse.EventButton(EButton::Left, EInputState::Down)
		.RemoveListener(UDelegate(this, &WButton::OnMouseLeftButtonDown));

	WWidget::Cleanup();
}

bool WButton::ReceiveMouseLeftButtonPressed()
{
	if (!IsMouseHovering())
	{
		return false;
	}

	isPressed_ = true;

	if (buttonSettings_.onPress)
	{
		buttonSettings_.onPress();
	}

	return true;
}

bool WButton::ReceiveMouseLeftButtonReleased()
{
	if (!isPressed_)
	{
		return false;
	}

	isPressed_ = false;

	if (buttonSettings_.onClick)
	{
		buttonSettings_.onClick();
	}

	return true;
}

void WButton::OnMouseLeftButtonPressedNoInteract()
{
	if (buttonSettings_.onPressOut)
	{
		buttonSettings_.onPressOut();
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
