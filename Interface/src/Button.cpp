#include "Button.h"
#include "Interface.h"
#include <kotono_common/log.h>
#include <kotono_input/Mouse.h>

WButton::WButton(const ButtonSettings& buttonSettings) 
	: buttonSettings_(buttonSettings)
{
	Interface.AddButton(this);

	Mouse.EventButton(EButton::Left, EInputState::Down).AddListener(this, &WButton::OnMouseLeftButtonDown);
}

void WButton::Cleanup()
{
	Interface.RemoveButton(this);

	Mouse.EventButton(EButton::Left, EInputState::Down).RemoveListener(this, &WButton::OnMouseLeftButtonDown);

	WWidget::Cleanup();
}

bool WButton::ReceiveMouseLeftButtonPressed()
{
	if (!IsMouseHovering())
	{
		return false;
	}

	isPressed_ = true;

	if (buttonSettings_.onPressed)
	{
		buttonSettings_.onPressed();
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

	if (buttonSettings_.onClicked)
	{
		buttonSettings_.onClicked();
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
