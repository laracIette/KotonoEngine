#include "Button.h"
#include "Interface.h"
#include <kotono_input/Mouse.h>

WButton::WButton()
{
	Interface.AddButton(Ptr());

	Mouse.EventButton(EButton::Left, EInputState::Down).AddListener(this, &WButton::OnMouseLeftButtonDown);
}

WButton::~WButton()
{
	Interface.RemoveButton(Ptr());

	Mouse.EventButton(EButton::Left, EInputState::Down).RemoveListener(this, &WButton::OnMouseLeftButtonDown);
}

bool WButton::ReceiveMouseLeftButtonPressed()
{
	if (!IsMouseHovering())
	{
		return false;
	}

	isPressed_ = true;

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

	if (onClicked_)
	{
		onClicked_();
	}

	return true;
}

void WButton::OnMouseLeftButtonPressedNoInteract()
{
	if (onPressOut_)
	{
		onPressOut_();
	}
}

const VoidCallback& WButton::GetOnClicked() const
{
	return onClicked_;
}

const VoidCallback& WButton::GetOnPressed() const
{
	return onPressed_;
}

const VoidCallback& WButton::GetOnDown() const
{
	return onDown_;
}

const VoidCallback& WButton::GetOnPressOut() const
{
	return onPressOut_;
}

void WButton::SetOnClicked(const VoidCallback& function)
{
	onClicked_ = function;
}

void WButton::SetOnPressed(const VoidCallback& function)
{
	onPressed_ = function;
}

void WButton::SetOnDown(const VoidCallback& function)
{
	onDown_ = function;
}

void WButton::SetOnPressOut(const VoidCallback& function)
{
	onPressOut_ = function;
}

void WButton::OnMouseLeftButtonDown()
{
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
