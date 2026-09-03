#include "Button.h"

WButton::WButton()
	: isEnabled_{ true }
	, isPressed_{ false }
{
}

b8 WButton::OnMouseButton(EButton button, EInputState inputState, glm::vec2 const& position)
{
	if (button != EButton::Left)
	{
		return INPUT_UNHANDLED;
	}

	switch (inputState)
	{
	case EInputState::Pressed:
	{
		isPressed_ = true;

		if (onActive_)
		{
			onActive_();
		}

		if (onPressed_)
		{
			onPressed_();
		}

		return INPUT_HANDLED;
	}
	case EInputState::Released:
	{
		if (!isPressed_)
		{
			break;
		}

		isPressed_ = false;

		if (onInactive_)
		{
			onInactive_();
		}

		if (onClicked_)
		{
			onClicked_();
		}

		return INPUT_HANDLED;
	}
	case EInputState::Down:
	{
		if (!isEnabled_)
		{
			break;
		}

		if (!isPressed_)
		{
			break;
		}

		if (onDown_)
		{
			onDown_();
		}

		return INPUT_HANDLED;
	}
	default:
		break;
	}
	

	return INPUT_UNHANDLED;
}

b8 WButton::OnMouseMove(glm::vec2 const& delta, glm::vec2 const& position)
{
	return INPUT_UNHANDLED;
}

void WButton::OnUnfocused()
{
	if (isPressed_)
	{
		isPressed_ = false;

		if (onInactive_)
		{
			onInactive_();
		}
	}
}

#include "generated/Button.generated.inl"
