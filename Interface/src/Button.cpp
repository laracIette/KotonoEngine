#include "Button.h"
#include "Interface.h"
#include <kotono_input/Mouse.h>

WButton::WButton()
	: isEnabled_{ true }
{
	Interface.AddButton(Ptr());
}

WButton::~WButton()
{
	Interface.RemoveButton(Ptr());
}

void WButton::Display(UWidgetDisplaySettings const& displaySettings)
{
	Base::Display(displaySettings);

	Mouse.EventButton(EButton::Left, EInputState::Down).AddListener(this, &WButton::OnMouseLeftButtonDown);
}

void WButton::Remove()
{
	Base::Remove();

	Mouse.EventButton(EButton::Left, EInputState::Down).RemoveListener(this, &WButton::OnMouseLeftButtonDown);
}

bool WButton::ReceiveMouseLeftButtonPressed()
{
	if (!isEnabled_)
	{
		return false;
	}

	if (!IsMouseHovering())
	{
		return false;
	}

	isPressed_ = true;

	if (onActive_)
	{
		onActive_();
	}

	if (onPressed_)
	{
		onPressed_();
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

	if (onInactive_)
	{
		onInactive_();
	}

	if (!IsMouseHovering())
	{
		return false;
	}

	if (onClicked_)
	{
		onClicked_();
	}

	return true;
}

void WButton::OnMouseLeftButtonPressedNoInteract()
{
	if (!isEnabled_)
	{
		return;
	}

	if (onPressOut_)
	{
		onPressOut_();
	}
}

void WButton::OnMouseLeftButtonDown()
{
	if (!isEnabled_)
	{
		return;
	}

	if (!isPressed_)
	{
		return;
	}

	if (onDown_)
	{
		onDown_();
	}
}

#include "generated/Button.generated.inl"
